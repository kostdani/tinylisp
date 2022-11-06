
(define (findmax l m)
(if (null? l) m
(if (or (not m) (> (car l) m)) (findmax (cdr l) (car l)) (findmax (cdr l) m))))

(define (remelem l e)
(if (= (car l) e) (cdr l) (cons (car l) (remelem (cdr l) e)))
)

(define (contains? l e)
(if (null? l) #f
(if (= (car l) e) #t
(contains? (cdr l) e))))

(define (recintersect a b r)
(if (null? a) r
(let ((max (findmax a #f)))
(recintersect (remelem a max) (if (contains? b max) (remelem b max) b) (if (contains? b max) (cons max r) r)))))

(define (intersect a b) (recintersect a b '()))
