
(define (len l)
  (foldl (lambda (x y) (+ 1 y)) 0 l))
(define (intervals l)
(map (lambda (x) (filter x l))
(map (lambda (a) (lambda (b) (and (<= (car a) (cadr b)) (>= (cadr a) (car b))))) l)))