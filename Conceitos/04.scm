#lang plai-typed

; Closure - cada função precisa de um environment como componente adicional
;           que se mantém para funções definidas dentro de uma ExprC

(define-type ExprC
  [numC  (n : number)]
  [idC   (s : symbol)]
  [plusC (l : ExprC) (r : ExprC)]
  [multC (l : ExprC) (r : ExprC)]
  [lamC  (arg : symbol) (body : ExprC)] ; nomes não são mais necessários
  [appC  (fun : ExprC) (arg : ExprC)]
  [ifC   (condition : ExprC) (yes : ExprC) (no : ExprC)]
  )

(define-type ExprS
  [numS    (n : number)]
  [idS     (s : symbol)]
  [lamS    (arg : symbol) (body : ExprS)] ; muda de acordo
  [appS    (fun : ExprS) (arg : ExprS)]
  [plusS   (l : ExprS) (r : ExprS)]
  [multS   (l : ExprS) (r : ExprS)]
  [bminusS (l : ExprS) (r : ExprS)]
  [uminusS (e : ExprS)]
  [ifS     (c : ExprS) (y : ExprS) (n : ExprS)]
  )

(define (desugar [as : ExprS]) : ExprC
  (type-case ExprS as
    [numS    (n) (numC n)]
    [idS     (s) (idC s)]
    [lamS    (a b) (lamC a (desugar b))] ; muda de acordo também
    [appS    (fun arg) (appC (desugar fun) (desugar arg))]
    [plusS   (l r) (plusC (desugar l) (desugar r))]
    [multS   (l r) (multC (desugar l) (desugar r))]
    [bminusS (l r) (plusC (desugar l) (multC (numC -1) (desugar r)))]
    [uminusS (e) (multC (numC -1) (desugar e))]
    [ifS     (c y n) (ifC (desugar c) (desugar y) (desugar n))]
    ))

(define-type Value
  [numV (n : number)]
  [closV (arg : symbol) (body : ExprC) (env : Env)])

(define-type Binding
  [bind (name : symbol) (val : Value)])

(define-type-alias Env (listof Binding))
(define mt-env empty)
(define extend-env cons)

(define (num+ [l : Value] [r : Value]) : Value
  (cond
    [(and (numV? l) (numV? r))
     (numV (+ (numV-n l) (numV-n r)))]
    [else
     (error 'num+ "Um dos argumentos não é um número")]))
(define (num* [l : Value] [r : Value]) : Value
  (cond
    [(and (numV? l) (numV? r))
     (numV (* (numV-n l) (numV-n r)))]
    [else
     (error 'num* "Um dos argumentos não é um número")]))

(define (interp [a : ExprC] [env : Env]) : Value
  (type-case ExprC a
    [numC (n) (numV n)]
    [idC (n) (lookup n env)]
    [lamC (a b) (closV a b env)] ; definição de função captura o environment
    [appC (f a)
          (local ([define f-value (interp f env)])
            (interp (closV-body f-value)
                    (extend-env
                      (bind (closV-arg f-value) (interp a env))
                      (closV-env f-value)
                    )))]
    [plusC (l r) (num+ (interp l env) (interp r env))]
    [multC (l r) (num* (interp l env) (interp r env))]
    [ifC (c y n) (if (zero? (numV-n (interp c env))) (interp y env) (interp n env))]
    ))

(define (lookup [for : symbol] [env : Env]) : Value
  (cond
    [(empty? env) (error 'lookup (string-append (symbol->string for) " não foi encontrado"))] ; environment livre (não definido)
    [else (cond
            [(symbol=? for (bind-name (first env))) ; encontrou
                           (bind-val (first env))]
            [else (lookup for (rest env))])]))

(define (parse [s : s-expression]) : ExprS
  (cond
    [(s-exp-number? s) (numS (s-exp->number s))]
    [(s-exp-symbol? s) (idS (s-exp->symbol s))] ; pode ser um símbolo livre nas definições de função
    [(s-exp-list? s)
     (let ([sl (s-exp->list s)])
       (case (s-exp->symbol (first sl))
         [(+) (plusS (parse (second sl))
                     (parse (third sl)))]
         [(*) (multS (parse (second sl))
                     (parse (third sl)))]
         [(-) (bminusS (parse (second sl))
                       (parse (third sl)))]
         [(~) (uminusS (parse (second sl)))]
         [(func) (lamS (s-exp->symbol (second sl)) (parse (third sl)))] ; definição
         [(call) (appS (parse (second sl))
                       (parse (third sl)))]
         [(if) (ifS (parse (second sl))
                    (parse (third sl))
                    (parse (fourth sl)))]
         [else (error 'parse "invalid list input")]))]
    [else (error 'parse "invalid input")]))

; facilitador
(define (interpS [s : s-expression]) (interp (desugar (parse s)) mt-env))

; Testes
(test (interp (plusC (numC 10) (appC (lamC '_ (numC 5)) (numC 10)))
              mt-env)
      (numV 15))
(interpS '(+ 10 (call (func x (+ x x)) 16)))
(interpS '(func f1 x (func f2 x (+ x x))))