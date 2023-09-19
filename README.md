# TinyLISP interpreter
TinyLisp is an implementation of a very small lisp-like language. It contains a small but functional lexer, parser, runtime, garbage collector, SECD translator and its VM.

## Example usage
the application can:

translate and interpret basic LISP constructions: symbols, integer and string atoms, pairs, quote, if, lambda
support all SECD instructions (except RAP): builtin instruction, LDC, SEL, JOIN, LD, LDF, AP, RET
Run the application on the command line.

examples of supported programs:

( - 10 ( + 2 ( * 4 5)))

(if 0 (+ 2 3) (+ 4 5))

(* (if 0 (+ 2 3) (+ 4 5)) 10)

(+ 1 ((lambda (x y) (+ x y)) 10 20))

((if 0 (lambda (x y) (+ x y)) (lambda (x y) (- x y))) 5 4)
