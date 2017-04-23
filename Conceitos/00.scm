#lang plai-typed

; Primeira linguagem - "calculadora"
; Tem apenas operadores aritméticas básicos e condicional simples (if)
; (soma, subtração, multiplicação, divisão, negação e condicional)

; Contém as operações primitivas
(define-type ArithC
  [numC (n : number)]
  [plusC (l : ArithC) (r : ArithC)]
  [multC (l : ArithC) (r : ArithC)]
  [ifC   (condition : ArithC) (yes : ArithC) (no : ArithC)])

; Contém operações que são composições das operações de ArithC
(define-type ArithS
  [numS    (n : number)]
  [plusS   (l : ArithS) (r : ArithS)]
  [bminusS (l : ArithS) (r : ArithS)]
  [uminusS (e : ArithS)]
  [multS   (l : ArithS) (r : ArithS)]
  [ifS     (c : ArithS) (y : ArithS) (n : ArithS)]
  )

(define (parse [s : s-expression]) : ArithS
  (cond
    [(s-exp-number? s) (numS (s-exp->number s))]
    [(s-exp-list? s)
     (let ([sl (s-exp->list s)])
       (case (s-exp->symbol (first sl))
         [(+)  (plusS (parse (second sl)) (parse (third sl)))]
         [(*)  (multS (parse (second sl)) (parse (third sl)))]
         [(-)  (bminusS (parse (second sl)) (parse (third sl)))]
         [(~)  (uminusS (parse (second sl)))]
         [(if) (ifS (parse (second sl)) (parse (third sl)) (parse (fourth sl)))]
         [else (error 'parse "invalid list input")]))]
    [else (error 'parse "invalid input")]))

(define (desugar [as : ArithS]) : ArithC
  (type-case ArithS as
    [numS    (n)     (numC n)]
    [plusS   (l r)   (plusC (desugar l) (desugar r))]
    [multS   (l r)   (multC (desugar l) (desugar r))]
    [bminusS (l r)   (plusC (desugar l) (multC (numC -1) (desugar r)))]
    [uminusS (e)     (multC (numC -1)   (desugar e))]
    [ifS     (c s n) (ifC   (desugar c) (desugar s) (desugar n))]
    ))

(define (interp [a : ArithC]) : number
  (type-case ArithC a
    [numC (n) n]
    [plusC (l r)   (+ (interp l) (interp r))]
    [multC (l r)   (* (interp l) (interp r))]
    [ifC   (c s n) (if (zero? (interp c)) (interp s) (interp n))]
    ))

(define (interpS [a : ArithS]) (interp (desugar a)))

;(parse '(if (- 3 2) 42 (+ 5 8)))
(interpS (parse '(if (- 3 2) 42 (+ 5 8))))
(interpS (parse '(if (- 3 3) 42 (+ 5 8))))
(interpS (parse '(if (- 3 4) 42 (+ 5 8))))




