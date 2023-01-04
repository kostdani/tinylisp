# Interpreter TinyLISP

TinyLisp je implementace velmi malého lisp-like jazyka. Obsahuje malý, ale funkční, lexer, parser, runtime, garbage kolektor, SECD překladač a jeho VM.

## Example usage

aplikace umí:

* překládat a interpretovat základní LISPovské konstrukce:
symboly, celočíselné a řetězcové atomy, páry, quote, if, lambda
* podporovat všechny SECD instrukce (kromě RAP):
builtin instrukce, LDC, SEL, JOIN, LD, LDF, AP, RET

Aplikace spusti na příkazovém řádku.

příklady podporovaných programů:

( - 10 ( + 2 ( * 4 5)))

(if 0 (+ 2 3) (+ 4 5))

(* (if 0 (+ 2 3) (+ 4 5)) 10)

(+ 1 ((lambda (x y) (+ x y)) 10 20))

((if 0 (lambda (x y) (+ x y)) (lambda (x y) (- x y))) 5 4)
