
(define (matrix-mul a b)
(map (lambda (ae)
(foldl (lambda (x y) (map + x y)) (map (lambda (x) 0) (car b))
(map (lambda (x y) (map (lambda (z) (* x z)) y)) ae b))) a))
