#lang plai-typed

((lambda (x) : number (+ x 1)) 10)
((lambda (x) (+ x 5)) 11)
(define mais2 : (number -> number) (lambda (x) (+ x 2)))
(mais2 5)
(define soma2 (lambda ([x : number]) : number (+ x 2)))
(soma2 7)
(define (s2 [x : number]) : number (+ x 2))
(s2 9)

(define (cria-somador n)
  (lambda (m)
    (+ m n)))
(cria-somador 8)
(define soma-5 (cria-somador 5))
(soma-5 12)
((cria-somador 4) 12)

(map (lambda (x) (* x x)) (list 1 2 3))
(filter (lambda (x) (> x 5)) (list -2 0 321 1 4 90))

(define [mn [m : number] [n : number]] (+ m n))
(mn 21 42)

(define (m) : number 5)
(m)

(local
  ([define v 20]
   [define l (lambda () 22)])
  (+ v (l))
)

(let ((x 10)
      (y 11))
  (+ x y)
)