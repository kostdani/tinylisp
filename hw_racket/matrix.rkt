
(define (matrix-mul a b)
(if (and (equal? a b) (equal? a '(()))) a
(map (lambda (ae)
(foldl (lambda (x y) (map + x y)) (map (lambda (x) 0) a)
(map (lambda (x y) (map (lambda (z) (* x z)) y)) ae b))) a)))
