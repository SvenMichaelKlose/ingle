#ifndef __CBM__
#define __CBM__
#endif

#include <ingle/cc65-charmap.h>
#include <cbm.h>

#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include <simpleio/libsimpleio.h>
#include <lisp/liblisp.h>

#ifdef __CC65__
#pragma bss-name (push, "ZEROPAGE")
#endif
lispptr arg1;
lispptr arg2c;
lispptr arg2;
#ifdef __CC65__
#pragma zpsym ("arg1")
#pragma zpsym ("arg2c")
#pragma zpsym ("arg2")
#pragma bss-name (pop)
#endif

extern void error (char * msg);

void
error (char * msg)
{
    errouts ("ERROR: ");
    outs (msg);
    outs ("\n\r");
    while (1);
}

void
bierror (char * msg)
{
    bierror (msg);
}

void FASTCALL
bi_1arg (lispptr x, char * msg)
{
    if (!CONSP(x)
        || !NOT(CDR(x)))
        bierror (msg);
    arg1 = eval (CAR(x));
}

void FASTCALL
bi_2args (lispptr x, char * msg)
{
    if (!CONSP(x)
        || NOT(CONSP(arg2c = CDR(x)))
        || !NOT(CDR(arg2c)))
        bierror (msg);
    arg1 = eval (CAR(x));
    PUSH(arg1);
    arg2 = eval (CAR(arg2c));
    POP(arg1);
}

lispptr FASTCALL
bi_eq (lispptr x)
{
    bi_2args (x, "(eq x x)");
    return arg1 == arg2 ? t : nil;
}

lispptr FASTCALL
bi_not (lispptr x)
{
    bi_1arg (x, "(not x)");
    return NOT(arg1) ? t : nil;
}

lispptr FASTCALL
bi_atom (lispptr x)
{
    bi_1arg (x, "(atom x)");
    return CONSP(arg1) ? nil : t;
}

lispptr FASTCALL
bi_symbolp (lispptr x)
{
    bi_1arg (x, "(symbol? x)");
    return SYMBOLP(arg1) ? t : nil;
}

lispptr FASTCALL
bi_setq (lispptr x)
{
    if (!CONSP(x)
        || !CONSP(arg2c = LIST_CDR(x))
        || !SYMBOLP(arg1 = CAR(x))
        || !NOT(CDR(arg2c)))
        bierror ("(setq sym x)");
    SET_SYMBOL_VALUE(arg1, arg2 = eval (CAR(arg2c)));
    return arg2;
}

lispptr FASTCALL
bi_symbol_value (lispptr x)
{
    if (!CONSP(x)
        || !NOT((CDR(x)))
        || !SYMBOLP(arg1 = eval (CAR(x))))
        bierror ("(symbol-value symbol)");
    return SYMBOL_VALUE(arg1);
}

lispptr FASTCALL
bi_quote (lispptr x)
{
    if (!CONSP(x)
        || !NOT(CDR(x)))
        bierror ("(quote x)");
    return CAR(x);
}

lispptr FASTCALL
bi_consp (lispptr x)
{
    bi_1arg (x, "(cons? x)");
    return CONSP(arg1) ? t : nil;
}

lispptr FASTCALL
bi_cons (lispptr x)
{
    bi_2args (x, "(cons x x)");
    return lisp_make_cons(arg1, arg2);
}

void FASTCALL
cxr_args (lispptr x, char * msg)
{
    if (!CONSP(x)
        || !NOT((CDR(x)))
        || !LISTP(arg1 = eval (CAR(x))))
        bierror (msg);
}

lispptr FASTCALL
bi_car (lispptr x)
{
    cxr_args (x, "(car lst)");
    return LIST_CAR(arg1);
}

lispptr FASTCALL
bi_cdr (lispptr x)
{
    cxr_args (x, "(cdr lst)");
    return LIST_CDR(arg1);
}

void FASTCALL
rplac_args (lispptr x, char * msg)
{
    if (!CONSP(x))
        bierror (msg);
    arg1 = eval (CAR(x));
    PUSH(arg1);
    if (!NOT(arg2c = eval (CDR(x)))
        || !NOT(CDR(arg2c))
        || !CONSP(arg2 = CAR(arg2c)))
        bierror (msg);
    POP(arg1);
}

lispptr FASTCALL
bi_rplaca (lispptr x)
{
    rplac_args (x, "(rplaca x cons)");
    return RPLACA(arg1, arg2);
}

lispptr FASTCALL
bi_rplacd (lispptr x)
{
    rplac_args (x, "(rplacd x cons)");
    return RPLACD(arg1, arg2);
}

lispptr FASTCALL
bi_numberp (lispptr x)
{
    bi_1arg (x, "(number? x)");
    return NUMBERP(CAR(x)) ? t : nil;
}

void FASTCALL
bi_arith_arg (lispptr x, char * msg)
{
    if (!CONSP(x)
        || !NUMBERP(arg1 = eval (CAR(x)))
        || !NOT(CDR(x)))
        bierror (msg);
}

void FASTCALL
bi_arith_args (lispptr x, char * msg)
{
    if (!CONSP(x)
        || !NUMBERP(arg1 = CAR(x))
        || NOT(arg2c = CDR(x))
        || !NUMBERP(arg2 = CAR(arg2c))
        || !NOT(CDR(arg2c)))
        bierror (msg);
}

void FASTCALL
bi_arith_many (lispptr x, char * msg)
{
    if (!CONSP(x)
        || !NUMBERP(arg1 = eval (CAR(x)))
        || NOT(arg2c = CDR(x)))
        bierror (msg);
}

lispptr FASTCALL
bi_equal (lispptr x)
{
    bi_arith_args (x, "(== n n)");
    return BOOL(NUMBER_VALUE(arg1) == NUMBER_VALUE(arg2));
}

lispptr FASTCALL
bi_lt (lispptr x)
{
    bi_arith_args (x, "(< n n)");
    return BOOL(NUMBER_VALUE(arg1) < NUMBER_VALUE(arg2));
}

lispptr FASTCALL
bi_lte (lispptr x)
{
    bi_arith_args (x, "(<= n n)");
    return BOOL(NUMBER_VALUE(arg1) <= NUMBER_VALUE(arg2));
}

lispptr FASTCALL
bi_gt (lispptr x)
{
    bi_arith_args (x, "(> n n)");
    return BOOL(NUMBER_VALUE(arg1) > NUMBER_VALUE(arg2));
}

lispptr FASTCALL
bi_gte (lispptr x)
{
    bi_arith_args (x, "(>= n n)");
    return BOOL(NUMBER_VALUE(arg1) >= NUMBER_VALUE(arg2));
}

#define DOLIST(x, init) \
    for (x = init; !NOT(x); x = LIST_CDR(x))

#define DEFARITH(fun_name, op, err) \
lispptr FASTCALL \
fun_name (lispptr x) \
{ \
    static char * msg = err; \
    int v; \
    lispptr n; \
    bi_arith_many (x, err); \
    v = NUMBER_VALUE(arg1); \
    DOLIST(x, arg2c) { \
        PUSH(x); \
        if (!NUMBERP(n = eval (CAR(x)))) \
            bierror (msg); \
        POP(x); \
        v op NUMBER_VALUE(n); \
    } \
    return lisp_make_number (v); \
}

DEFARITH(bi_add, +=, "(+ n n...)");
DEFARITH(bi_sub, -=, "(- n n...)");
DEFARITH(bi_mul, *=, "(* n n...)");
DEFARITH(bi_div, /=, "(/ n n...)");
DEFARITH(bi_mod, %=, "(% n n...)");

lispptr FASTCALL
bi_inc (lispptr x)
{
    bi_arith_arg (x, "(++ n)");
    return lisp_make_number (NUMBER_VALUE(arg1) + 1);
}

lispptr FASTCALL
bi_dec (lispptr x)
{
    bi_arith_arg (x, "(-- n)");
    return lisp_make_number (NUMBER_VALUE(arg1) - 1);
}

lispptr FASTCALL
bi_bit_and (lispptr x)
{
    bi_arith_args (x, "(bit-and n n)");
    return lisp_make_number (NUMBER_VALUE(arg1) & NUMBER_VALUE(arg2));
}

lispptr FASTCALL
bi_bit_or (lispptr x)
{
    bi_arith_args (x, "(bit-or n n)");
    return lisp_make_number (NUMBER_VALUE(arg1) | NUMBER_VALUE(arg2));
}

lispptr FASTCALL
bi_bit_xor (lispptr x)
{
    bi_arith_args (x, "(bit-xor n n)");
    return lisp_make_number (NUMBER_VALUE(arg1) ^ NUMBER_VALUE(arg2));
}

lispptr FASTCALL
bi_bit_neg (lispptr x)
{
    bi_arith_arg (x, "(bit-neg n)");
    return lisp_make_number (~NUMBER_VALUE(arg1));
}

lispptr FASTCALL
bi_shift_left (lispptr x)
{
    bi_arith_args (x, "(<< n nbits)");
    return lisp_make_number (NUMBER_VALUE(arg1) << NUMBER_VALUE(arg2));
}

lispptr FASTCALL
bi_shift_right (lispptr x)
{
    bi_arith_args (x, "(>> n nbits)");
    return lisp_make_number (NUMBER_VALUE(arg1) >> NUMBER_VALUE(arg2));
}

lispptr FASTCALL
bi_peek (lispptr x)
{
    bi_arith_arg (x, "(peek addr)");
    return lisp_make_number (*(char *) NUMBER_VALUE(arg1));
}

lispptr FASTCALL
bi_poke (lispptr x)
{
    bi_arith_args (x, "(poke addr b)");
    *(char *) NUMBER_VALUE(arg1) = NUMBER_VALUE(arg2);
    return arg2;
}

lispptr FASTCALL
bi_sys (lispptr x)
{
    bi_arith_arg (x, "(sys addr)");
    ((void (*) (void)) NUMBER_VALUE(arg1)) ();
    return nil;
}

lispptr FASTCALL
bi_eval (lispptr x)
{
    bi_1arg (x, "(eval x)");
    return eval (arg1);
}

lispptr FASTCALL
bi_apply (lispptr x)
{
    if (!CONSP(x)
        || NOT(arg2c = CDR(x))
        || !NOT(CDR(arg2c)))
        bierror ("(apply fun . args)");
    return apply (CAR(x), arg2c, true);
}

lispptr return_tag;
lispptr go_tag;

lispptr FASTCALL
bi_block (lispptr x)
{
    lispptr res;
    lispptr p;
    lispptr tag;

    if (!CONSP(x)
        || !SYMBOLP(arg1 = CAR(x))
        || !NOT(arg2c = CDR(x))
        || !NOT(CDR(arg2c)))
        bierror ("(block name . exprs)");

    DOLIST(p, arg2) {
        PUSH(p);
        res = eval (CAR(p));
        POP(p);
        if (CONSP(res)) {
            // Handle RETURN.
            if (CAR(res) == return_tag) {
                if (arg1 == CAR(CDR(res)))
                    return CDR(CDR(res));
                return res;
            }

            // Handle GO.
            if (CAR(res) == go_tag) {
                // Search tag in body.
                tag = CAR(CDR(res));
                for (p = arg2; !NOT(p); p = LIST_CDR(p)) {
                    if (CAR(p) == tag) {
                        p = CDR(p);
                        break;
                    }
                }
                return res;
            }
        }
    }
    return res;
}

lispptr FASTCALL
bi_return (lispptr x)
{
    if (!CONSP(x))
        bierror ("(return x [name])");
    // TODO: Re-use list.
    arg1 = eval (CAR(x));
    PUSH(arg1);
    arg2 = eval (LIST_CAR(LIST_CDR(x)));
    POP(arg1);
    arg1 = lisp_make_cons (arg1, arg2);
    return lisp_make_cons (return_tag, arg1);
}

lispptr FASTCALL
bi_go (lispptr x)
{
    bi_1arg (x, "(go tag)");
    // TODO: Re-use cons.
    return lisp_make_cons (go_tag, arg1);
}

lispptr tmp;

lispptr FASTCALL
bi_if (lispptr x)
{
    if (!CONSP(x)
        || NOT(CONSP(arg2c = CDR(x)))
        || NOT(CDR(arg2c)))
        bierror ("(? cond x [cond x/default])");
    while (!NOT(x)) {
        arg1 = CAR(x);
        if (NOT(arg2c = CDR(x)))
            return eval (arg1);
        PUSH(arg2c);
        tmp = eval (arg1);
        POP(arg2c);
        if (!NOT(tmp))
            return eval (CAR(arg2c));
        x = CDR(arg2c);
    }
    // NOTREACHED, I hope...
    bierror ("?: default missing.");
}

lispptr FASTCALL
bi_and (lispptr x)
{
    DOLIST(x, x) {
        PUSH(x);
        if (NOT(eval (CAR(x)))) {
            POP(x);
            return nil;
        }
        POP(x);
    }
    return t;
}

lispptr FASTCALL
bi_or (lispptr x)
{
    DOLIST(x, x) {
        PUSH(x);
        if (!NOT(eval (CAR(x)))) {
            POP(x);
            return t;
        }
        POP(x);
    }
    return nil;
}

lispptr FASTCALL
bi_read (lispptr x)
{
    if (!NOT(x))
        bierror ("(read)");
    return lisp_read ();
}

lispptr FASTCALL
bi_print (lispptr x)
{
    bi_1arg (x, "(print x)");
    return lisp_print (arg1);
}

lispptr FASTCALL
bi_fn (lispptr x)
{
    if (!CONSP(x)
        || !SYMBOLP(arg1 = CAR(x))
        || !CONSP(arg2c = CDR(x)))
        bierror ("(fn name obj)");
    EXPAND_UNIVERSE(arg1);
    SET_SYMBOL_VALUE(arg1, arg2c);
    return nil;
}

lispptr FASTCALL
bi_var (lispptr x)
{
    if (!CONSP(x)
        || !SYMBOLP(arg1 = CAR(x))
        || NOT(CONSP(arg2c = CDR(x)))
        || !NOT(CDR(arg2c)))
        bierror ("(var name obj)");
    EXPAND_UNIVERSE(arg1);
    SET_SYMBOL_VALUE(arg1, eval (CAR(arg2c)));
    return nil;
}

lispptr FASTCALL
bi_gc (lispptr x)
{
    (void) x;
    gc ();
    return lisp_make_number (heap_end - heap_free);
}

struct builtin builtins[] = {
    { "quote",      bi_quote },

    { "apply",      bi_apply },
    { "eval",       bi_eval },

    { "?",          bi_if },
    { "&",          bi_and },
    { "|",          bi_or },
    { "block",      bi_block },
    { "return",     bi_return },
    { "go",         bi_go },

    { "not",        bi_not },
    { "eq",         bi_eq },
    { "atom",       bi_atom },
    { "cons?",      bi_consp },
    { "number?",    bi_numberp },
    { "symbol?",    bi_symbolp },

    { "setq",         bi_setq },
    { "symbol-value", bi_symbol_value },

    { "cons",       bi_cons },
    { "car",        bi_car },
    { "cdr",        bi_cdr },
    { "rplaca",     bi_rplaca },
    { "rplacd",     bi_rplacd },

    { "==",         bi_equal },
    { ">",          bi_gt },
    { "<",          bi_lt },
    { ">=",         bi_gte },
    { "<=",         bi_lte },

    { "+",          bi_add },
    { "-",          bi_sub },
    { "*",          bi_mul },
    { "/",          bi_div },
    { "%",          bi_mod },
    { "++",         bi_inc },
    { "--",         bi_dec },

    { "bit-and",    bi_bit_and },
    { "bit-or",     bi_bit_or },
    { "bit-xor",    bi_bit_xor },
    { "bit-neg",    bi_bit_neg },
    { "<<",         bi_shift_left },
    { ">>",         bi_shift_right },

    { "peek",       bi_peek },
    { "poke",       bi_poke },
    { "sys",        bi_sys },

    { "read",       bi_read },
    { "print",      bi_print },

    { "fn",         bi_fn },
    { "var",        bi_var },
    { "gc",         bi_gc },

    { NULL, NULL }
};

void
init_builtins (void)
{
    return_tag = lisp_make_symbol ("%R", 2);
    go_tag = lisp_make_symbol ("%G", 2);
    EXPAND_UNIVERSE(return_tag);
    EXPAND_UNIVERSE(go_tag);
    add_builtins (builtins);
}

void
load_environment (void)
{
    lispptr x;

    outs ("\n\rLoading ENV.LISP...\n\r");
    cbm_open (3, 8, 3, "ENV.LISP");
    // TODO: Error check.
    cbm_k_chkin (3);
    while (x = lisp_read ()) {
        lisp_print (x);
        outs ("\n\r");
        //lisp_print (x);
        //outs ("\n\r");
        x = eval (x);
        lisp_print (x);
        outs ("\n\r");
        //lisp_print (x);
        //outs ("\n\r");
    }
    cbm_k_close (3);
    cbm_k_clrch ();
}

int
main (int argc, char * argv[])
{
    (void) argc, (void) argv;

    lisp_init ();
    init_builtins ();
    load_environment ();

    return 0;
}