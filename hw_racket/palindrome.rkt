(define (len l)
  (foldl (lambda (x y) (+ 1 y)) 0 l))
(define (rev l)
  (foldl cons '() l))
(define (cut l)
  (cdr (rev (cdr l))))

(define (is-palindrome? l)
  (if (>= 2 (len l)) #t
    (let ((l (cut l)))
      (equal? l (rev l)))))
