#lang plai-typed

; Funções passam a retornar valores (Value) ao invés de apenas números

(define-type ExprC
  [numC (n : number)]
  [idC  (s : symbol)]
  [plusC (l : ExprC) (r : ExprC)]
  [multC (l : ExprC) (r : ExprC)]
  [fdC (name : symbol) (arg : symbol) (body : ExprC)] ; declaração faz parte da expressão
  [appC (fun : ExprC) (arg : ExprC)] ; a aplicação recebe uma função
  [ifC (condition : ExprC) (yes : ExprC) (no : ExprC)]
  )

(define-type ExprS
  [numS    (n : number)]
  [idS     (s : symbol)]
  [fdS     (name : symbol) (arg : symbol) (body : ExprS)] ; declaração faz parte da expressão
  [appS    (fun : ExprS ) (arg : ExprS)]
  [plusS   (l : ExprS) (r : ExprS)]
  [bminusS (l : ExprS) (r : ExprS)]
  [uminusS (e : ExprS)]
  [multS   (l : ExprS) (r : ExprS)]
  [ifS     (c : ExprS) (y : ExprS) (n : ExprS)]
  )

(define (desugar [as : ExprS]) : ExprC
  (type-case ExprS as
    [numS    (n) (numC n)]
    [idS     (s) (idC s)]
    [fdS     (n a b) (fdC n a (desugar b))] ; deve converter o corpo
    [appS    (fun arg) (appC (desugar fun) (desugar arg))]
    [plusS   (l r) (plusC (desugar l) (desugar r))]
    [multS   (l r) (multC (desugar l) (desugar r))]
    [bminusS (l r) (plusC (desugar l) (multC (numC -1) (desugar r)))]
    [uminusS (e) (multC (numC -1) (desugar e))]
    [ifS     (c y n) (ifC (desugar c) (desugar y) (desugar n))]
    ))

(define-type Value
  [numV (n : number)]
  [funV (name : symbol) (arg : symbol) (body : ExprC)])

(define-type Binding
  [bind (name : symbol) (val : Value)])

(define-type-alias Env (listof Binding))
(define mt-env empty)
(define extend-env cons)

; Os resultados das operações aritméticas tiveram que ser adaptados para devolver numV
(define (num+ [l : Value] [r : Value]) : Value
  (cond
    [(and (numV? l) (numV? r)) (numV (+ (numV-n l) (numV-n r)))]
    [else (error 'num+ "Um dos argumentos não é um número")]))
(define (num* [l : Value] [r : Value]) : Value
  (cond
    [(and (numV? l) (numV? r)) (numV (* (numV-n l) (numV-n r)))]
    [else (error 'num* "Um dos argumentos não é um número")]))

(define (interp [a : ExprC] [env : Env]) : Value
  (type-case ExprC a
    [numC (n) (numV n)]
    [idC (n) (lookup n env)]
    [fdC (n a b) (funV n a b)]

    [appC (f a) (local ([define fd (interp f env)])
                  (interp (funV-body fd)
                          (extend-env (bind (funV-arg fd)
                                            (interp a env))
                                      mt-env)))]
    [plusC (l r) (num+ (interp l env) (interp r env))]
    [multC (l r) (num* (interp l env) (interp r env))]
    [ifC (c y n) (if (zero? (numV-n (interp c env))) (interp y env) (interp n env))]
    ))

(define (lookup [for : symbol] [env : Env])
  (cond
    [(empty? env) (error 'lookup (string-append (symbol->string for) " não foi encontrado"))] ; livre (não definida)
    [else (cond
          [(symbol=? for (bind-name (first env))) (bind-val (first env))] ; achou
          [else (lookup for (rest env))])])) ; vê no resto

(define (parse [s : s-expression]) : ExprS
  (cond
    [(s-exp-number? s) (numS (s-exp->number s))]
    [(s-exp-symbol? s) (idS  (s-exp->symbol s))] ; poder ser um símbolo livre nas definições da função
    [(s-exp-list? s)
     (let ([sl (s-exp->list s)])
       (case (s-exp->symbol (first sl))
         [(+)    (plusS   (parse (second sl))
                          (parse (third sl)))]
         
         [(*)    (multS   (parse (second sl))
                          (parse (third sl)))]
         
         [(-)    (bminusS (parse (second sl))
                          (parse (third sl)))]
         
         [(~)    (uminusS (parse (second sl)))]
         
         [(func) (fdS     (s-exp->symbol (second sl)) ; definição
                          (s-exp->symbol (third sl))
                          (parse         (fourth sl)))]
         
         [(call) (appS    (parse (second sl))
                          (parse (third sl)))]
         
         [else (error 'parse "invalid list input")]))]
    [else (error 'parse "invalid input")]))

; Para não precisar ficar colocando mt-env toda vez que chama o interp
(define (interpS [s : s-expression]) (interp (desugar (parse s)) mt-env))

; Testes
;; Testes do arquivo funcaoValor.scm
(test (interp (plusC (numC 10) (appC (fdC 'const5 '_ (numC 5)) (numC 10)))
              mt-env)
      (numV 15))
(interpS '(+ 10 (call (func dobra x (+ x x)) 16)))

;; Testes da apostila
(interpS '(func f1 x (func f2 x (+ x x)))) ; associação de x em f1 não se propaga para f2, pois os environments são diferentes
(interpS '(call (func f1 x (func f2 x (+ x x))) 4)) ; idem a função de cima
; (interpS '(call (call (func f1 x (func f2 y (+ x y))) 4) 5)) ; função dá erro, pois x não está definido