#ifdef __CC65__
#ifndef __CBM__
#define __CBM__
#endif

#include <ingle/cc65-charmap.h>
#include <cbm.h>
#endif

#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include <simpleio/libsimpleio.h>

#include "liblisp.h"

extern void error (char * msg);

lispptr universe;

#ifdef __CC65__
#pragma bss-name (push, "ZEROPAGE")
#endif
char *    heap_start;
char *    heap_free;
char *    heap_end;
lispptr       nil;
lispptr       t;
#ifdef __CC65__
#pragma zpsym ("heap_start");
#pragma zpsym ("heap_free");
#pragma zpsym ("heap_end");
#pragma zpsym ("nil")
#pragma zpsym ("t")
#pragma bss-name (pop)
#endif

#ifdef __CC65__
#pragma bss-name (push, "ZEROPAGE")
#endif
extern char do_putback;
#ifdef __CC65__
#pragma zpsym ("do_putback")
#pragma bss-name (pop)
#endif

unsigned lisp_sizes[] = {
    0,
    sizeof (cons),
    sizeof (number),
    sizeof (symbol),
    sizeof (symbol)
};

unsigned FASTCALL
objsize (char * x)
{
    uchar type = *x & 7; // TODO: constant
    unsigned s;
    if (type > TYPE_MAX) {
        errouts ("No size for type ");
        out_number (type);
        while (1);
    }
    s = lisp_sizes[type];
    if (*x & TYPE_NAMED)
        return s + SYMBOL(x)->len;
    return s;
}

#define MIN_RELOC_TABLE_SIZE \
    (sizeof (lispptr) + sizeof (unsigned))
#define NEEDS_GC() \
    (heap_free > heap_end - size - MIN_RELOC_TABLE_SIZE)

// Allocate vanilla object.
lispptr FASTCALL
alloc (uchar size, uchar type)
{
    char * r;

    if (NEEDS_GC()) {
        gc ();
        if (NEEDS_GC())
            error ("Out of heap.");
    }

    r = heap_free;
    *heap_free = type;
    heap_free += size;
    *heap_free = 0;

    return r;
}

lispptr FASTCALL
lisp_make_cons (lispptr car, lispptr cdr)
{
    cons * c = alloc (sizeof (cons), TYPE_CONS);
    c->car = car;
    c->cdr = cdr;
    return c;
}

lispptr FASTCALL
lisp_make_number (int x)
{
    number * n = alloc (sizeof (number), TYPE_NUMBER);
    n->value = x;
    return n;
}

void * FASTCALL
lookup_symbol (char * str, uchar len)
{
    char *    s = (char *) heap_start;
    char      type;
    symbol *  sym;

    // Check all objects until end of heap.
    while ((type = *s)) {
        if (type & TYPE_NAMED) {
            sym = (symbol *) s;

            // Return match.
            if (sym->len == len
                && !memcmp (s + sizeof (symbol), str, len))
                return s;

            // Jump over symbol + name.
            s += sizeof (symbol) + sym->len;
            continue;
        }

        // Jump over current object.
        s += objsize (s);
    }

    return NULL;
}

lispptr __fastcall__
lisp_make_symbol (char * str, uchar len)
{
    symbol * s;

    // Return existing.
    if ((s = lookup_symbol (str, len)))
        return s;

    // Alloc new.
    s = alloc (sizeof (symbol) + len, TYPE_SYMBOL);
    s->value = s;
    s->len = len;
    memcpy ((char *) s + sizeof (symbol), str, len);
    return s;
}

void
lisp_init ()
{
    size_t heap_size;

    // Init stack.
    stack_start = malloc (STACK_SIZE);
    if (!stack_start) {
        outs ("Out of memory for stack.");
        while (1);
    }
    stack_end = stack_start + STACK_SIZE;
    stack = stack_end;

    // Init heap.
    heap_size = _heapmaxavail ();
    heap_start = heap_free = malloc (heap_size);
    if (!heap_start) {
        outs ("Out of memory for heap.");
        while (1);
    }
    *heap_free = 0;
    heap_end = heap_start + heap_size;

    // Make truth.
    nil = lisp_make_symbol ("nil", 3);
    t   = lisp_make_symbol ("t", 1);
    universe = nil;
    EXPAND_UNIVERSE(t);

    // Init input.
    do_putback = false;
}