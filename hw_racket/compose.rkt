
(define (compose-chain l)
(foldr (lambda (x y) (lambda (z) (x (y z)))) (lambda (x) x) l))
