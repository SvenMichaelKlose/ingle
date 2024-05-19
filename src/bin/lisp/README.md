TUNIX Lisp interpreter
======================

This is an interpreter with mark-and-sweep garbage
collection.

# Data types

* number (16 bit)
* cons
* symbol (0-255 char name)
* string (1-256 chars)
* builtin

Functions are conses with the CAR holding the argument
definition and the CDR holding the body.

# Built-in functions

## Quoting
### (quote x)

## Evaluation and flow control
### (apply fun . args)
### (eval x)
### (? cond expr [cond expr/default])
### (& x...)
### (| x...)
### (block name x...), (return x block-name), (go tag)

## Predicates
### (not x)
### (eq x)
### (atom x)
### (cons? x)
### (symbol? x)
### (number? x)

## Symbols

Symbols have a name up to 255 bytes in length and a value.

### (setq symbol x): Set symbol value
### (symbol-value symbol x); Get symbol value

## Conses
### (car lst)
### (cdr lst)
### (rplaca x lst)
### (rplacd x lst)

## Numbers
### Comparing
#### (== n n)
#### (> n n)
#### (< n n)
#### (>= n n)
#### (<= n n)

### Arithmetics
#### (+ n n...)
#### (- n n...)
#### (\* n n...)
#### (/ n n...)
#### (% n n...)
#### (++ n)
#### (-- n)

### Bit manipulation
#### (bit-and n n)
#### (bit-or n n)
#### (bit-xor n n)
#### (bit-neg n)
#### (>> n nbits)
#### (<< n nbits)

## I/O
### (read)
### (print x)

## Low level
### (peek addr)

Reads byte from memory at 'addr', which must be a positive
integer.

### (poke addr byte)
Writes 'byte' to memory at 'addr'.  Both must be positive
integers.

### (sys addr)

## Special
### (fn name args body...)

Used to define functions.  Assigns the list starting with
'args' to the symbol value of 'name' unevaluated and expands
the universe by 'name'.

### (var name obj)

Used to define variables.  Evaluates 'obj', assigns the
result to the symbol value of 'name' and expands the
universe by 'name'.

## Miscellaneous
### (gc)

# Future extensions

Numbers from 1 to 255 are pointer-encoded and do not
require garbage collection.

Swap tail of call stack with secondary storage.

Type array (256 objects maximum).

Compressed cons (special type) with CDR pointing to previous
cons (which may also be compressed already, so there is a
variable number of bytes to step down).