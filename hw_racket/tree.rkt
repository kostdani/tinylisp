
(define (my-max x y)
(if (> x y) x y))
(define (my-abs x)
(if (< x 0) (* x -1) x))

(define (height l)
(if (null? l) 0
(+ 1 (my-max (height (cadr l)) (height (caddr l))))))

(define (is-balanced? l)
(or (null? l)
(and (<= (my-abs (- (height (cadr l)) (height (caddr l)))) 1)
     (is-balanced? (cadr l))
     (is-balanced? (caddr l)))))
