#include <ingle/cc65-charmap.h>

#include <stdlib.h>
#include <stdbool.h>

#include <lisp/liblisp.h>
#include <simpleio/libsimpleio.h>

extern void error (char *);

void debug (void) { }

char * stack_start;

#ifdef __CC65__
#pragma bss-name (push, "ZEROPAGE")
#endif
extern lispptr tmp;
lispptr x;
lispptr args;
char * stack;
char * stack_end;
char * tagstack;
lispptr name;
lispptr defs;
lispptr value;
struct builtin * bfun;
lispptr va;
lispptr delayed_eval;
bool lisp_break;
uchar c;
extern char * msg;
#ifdef __CC65__
#pragma zpsym ("tmp")
#pragma zpsym ("x")
#pragma zpsym ("args")
#pragma zpsym ("stack")
#pragma zpsym ("stack_end")
#pragma zpsym ("tagstack")
#pragma zpsym ("name")
#pragma zpsym ("defs")
#pragma zpsym ("value")
#pragma zpsym ("bfun")
#pragma zpsym ("va")
#pragma zpsym ("delayed_eval")
#pragma zpsym ("lisp_break")
#pragma zpsym ("c")
#pragma zpsym ("msg")
#pragma bss-name (pop)
#endif

extern void bierror (void);

// Type-check arguments to built-in function using their
// character-based argument type list.
void
bi_tcheck (lispptr x, uchar type)
{
    (void) x;
    (void) type;
    switch (type) {

    // Any object.
    case 'x':
        return;

    // Numbers.
    case 'n':
        if (!NUMBERP(x)) {
            msg = "Number expected.";
            bierror ();
            while (1);
        }
        return;
#ifndef NDEBUG
    default:
        outs ("Developer error: '");
        out (type);
        outs ("': unknown typedef");
        while (1);
#endif
    }
}

// Evaluate list to list of return values.
lispptr
eval_list (void)
{
    if (lisp_break)
        return nil;
    if (ATOM(x))
        return x;
    PUSH(x);
    x = CAR(x);
    va = eval ();
    POP(x);
    PUSH(va);
    x = CDR(x);
    tmp = eval_list ();
    POP(va);
    return lisp_make_cons (va, tmp);
}

#define PUSH_TAG(x)     (*--tagstack = (x))
#define POP_TAG(x)      ((x) = *tagstack++)
#define PUSH_TAGW(x) \
    do { \
        tagstack -= sizeof (lispptr); \
        *(lispptr *) tagstack = x; \
    } while (0)
#define POP_TAGW(x) \
    do { \
        x = *(lispptr *) tagstack; \
        tagstack += sizeof (lispptr); \
    } while (0)

#define TAG_DONE            0
#define TAG_BARG_NEXT       1
#define TAG_ARG_NEXT        2
#define TAG_ARG_LAST        3
#define TAG_CONTINUE_BODY   4
#define TAG_ARG             5

char * badef;
lispptr * a;
uchar na;
bool unevaluated;

lispptr
eval0 (void)
{
do_eval:
    // Evaluate atom.
    if (ATOM(x)) {
        value = SYMBOLP(x) ? SYMBOL_VALUE(x) : x;
        goto got_value;
    }

    // Evaluate function.
    PUSH(x);
    x = CAR(x);
    arg1 = eval ();
    POP(x);

    args = CDR(x);

    // Call built-in.
    if (BUILTINP(arg1)) {
        bfun = (struct builtin *) SYMBOL_VALUE(arg1);

        // Get built-in argument definition.
        if (!(badef = bfun->argdef)) {
            // No definition.  Call with unevaluated args.
            x = args;
            value = bfun->func ();
            goto got_value;
        }

        na = 0;

do_builtin_arg:
        c = *badef;

        // End of argument definition.
        if (!c) {
            // Complain if argument left.
            if (args) {
                msg = "Too many args to builtin:";
                bierror ();
                lisp_print (args);
                while (1);
                goto got_value;
            }

            // Pop evaluated to arg1 and arg2.
            if (na--) {
                POP(arg2);
                if (na--)
                    POP(arg1);
            }

            // And call the built-in...
            value = bfun->func ();
            goto got_value;
        }

        // Complain about missing argument.
        if (!args) {
            msg = "Missing args to builtin.";
            bierror ();
            goto got_value;
        }

        // Now be have the argument in the head of 'args'
        // and its wanted type in *badef.  Increment the
        // argument count.
        na++;

        // Quick deal with unevaluated argument.
        if (c == '\\') {
            badef++;
            value = CAR(args);
            goto save_builtin_arg_value;
        }

        // Save variables on the stacks.
        PUSH(args);
        PUSH_TAGW(badef);
        PUSH_TAG(na);

        // Prepare evaluation and return.
        x = CAR(args);
        PUSH_TAG(TAG_BARG_NEXT);
        goto do_eval;

        // Step to next argument.
next_builtin_arg:
        POP_TAG(na);
        POP_TAGW(badef);
        POP(args);

save_builtin_arg_value:
        // Ensure the type is wanted.
        bi_tcheck (value, *badef++);

        // Save evaluated value on the GC stack to move
        // it to 'arg1' and 'arg2' when finished with all
        // arguments.
        PUSH(value);

        // Step to next argument.
        args = CDR(args);
        goto do_builtin_arg;
    }

    // Ensure user-defined function.
    if (ATOM(arg1)) {
        error ("Function expected, not: ");
        lisp_print (arg1);
        terpri ();
        value = nil;
        goto got_value;
    }

    // Init argument list evaluation.
    PUSH_TAG(TAG_DONE);
    defs = FUNARGS(arg1);
    if (!defs && !args)
        goto start_body;

    // Evaluate arguments to user-defined function.
do_argument:
    // Error if length of argument list and definition
    // don't match.
    if (!defs || !args) {
        if (defs) {
            errouts ("Argument(s) missing: ");
            lisp_print (defs);
        } else {
            errouts ("Too many arguments: ");
            lisp_print (args);
        }
        lisp_break = true;
        value = nil;
        goto got_value;
    }
 
    // Rest of argument list. (consing)
    if (ATOM(defs)) {
        // Save old symbol value.
        PUSH(SYMBOL_VALUE(defs));
        PUSH(defs);
        PUSH_TAG(TAG_ARG);

        if (unevaluated) {
            value = x;
        } else {
            // Evaluate rest of arguments.
            PUSH(defs);
            PUSH(arg1);
            x = args;
            value = eval_list ();
            POP(arg1);
            POP(defs);
        }

        // Assign rest of arguments.
        SET_SYMBOL_VALUE(defs, value);
        goto start_body;
    }

    // Save old argument symbol value.
    name = CAR(defs);
    PUSH(SYMBOL_VALUE(name));
    PUSH(name);
    PUSH_TAG(TAG_ARG);

    if (unevaluated) {
        value = CAR(args);
    } else {
        // Save variable on the GC stack.
        PUSH(arg1);
        PUSH(defs);

        // Prepare round of evaluation.
        if (CDR(defs) || CDR(args)) {
            PUSH(args);
            PUSH_TAG(TAG_ARG_NEXT);
        } else
            PUSH_TAG(TAG_ARG_LAST);
        x = CAR(args);
        goto do_eval;

        // Step to next argument.
    next_arg:
        // Restore variables.
        POP(args);
        POP(defs);
        POP(arg1);
    }

    // Replace argument symbol value with evaluated one.
    name = CAR(defs);
    SET_SYMBOL_VALUE(name, value);

    // Step to next argument.
    defs = CDR(defs);
    args = CDR(args);
    goto do_argument;

    // Handle last argument.
arg_last:
    // Restore variables.
    POP(defs);
    POP(arg1);

    // Replace argument symbol value with evaluated one.
    name = CAR(defs);
    SET_SYMBOL_VALUE(name, value);

    // Eavluate body.
start_body:
    x = FUNBODY(arg1);

    // Evaluate body statement.
do_body:
    // Break if out of statements or other reason.
    if (!x || lisp_break)
        goto done_body;

    // Save rest of statements on the GC stack.
    PUSH(CDR(x));

    // Prepare new round of evaluation.
    PUSH_TAG(TAG_CONTINUE_BODY);
    x = CAR(x);
    goto do_eval;

next_in_body:
    // Step to next statement.
    POP(x);
    goto do_body;

done_body:
    // Restore argument symbol values.
    while (POP_TAG(c) == TAG_ARG) {
        POP(name);
        POP(SYMBOL_VALUE(name));
    }

#ifndef NDEBUG
    if (c != TAG_DONE) {
        errouts ("Internal error: ");
        out_number (c);
        outs (": 0 expected after restoring arguments.");
        while (1);
    }
#endif // #ifndef NDEBUG

    // Dispatch value based on tag.
got_value:
    if (value == delayed_eval)
        goto do_eval;
    POP_TAG(c);
    if (c != TAG_DONE) {
        switch (c) {
        case TAG_ARG_NEXT:
            goto next_arg;
        case TAG_BARG_NEXT:
            goto next_builtin_arg;
        case TAG_CONTINUE_BODY:
            goto next_in_body;
        case TAG_ARG_LAST:
            goto arg_last;
        }
#ifndef NDEBUG
        errouts ("Internal error: ");
        out_number (c);
        outs (": Unknown eval tag.");
        while (1);
#endif // #ifndef NDEBUG
    }
    return value;
}

lispptr
eval ()
{
    unevaluated = false;
    PUSH_TAG(TAG_DONE);
    return eval0 ();
}

lispptr
funcall ()
{
    unevaluated = true;
    PUSH_TAG(TAG_DONE);
    return eval0 ();
}
