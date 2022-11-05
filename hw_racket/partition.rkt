
(define (partition-by f l)
(cons (filter f l) (cons (filter (lambda (x) (not (f x))) l) '())))
