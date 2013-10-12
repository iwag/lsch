(define (fib n ) (if (= n 0 ) 0 (if (= n 1 ) 1 (+ (fib (- n 1 ) ) (fib (- n 2 ) ) ) ) ) )

(define (f n ) (if (= n 1 ) 1 (* n (f (- n 1 ) ) ) ) ) 

(fib 10)

(define (member? lst x) (if (null? lst) false (if (= (car lst) x) true (member? (cdr lst) x) ) ) )

(define hoge (list 1 2 3 4 8) )
(member? hoge 2)
(member? hoge 5)
(member? hoge 8)
