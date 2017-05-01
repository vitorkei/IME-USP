#lang plai-typed

(define-type ExprC
  [numC (n : number)]
  [idC     (s : symbol)]
  [plusC   (l : ExprC) (r : ExprC)]
  [multC   (l : ExprC) (r : ExprC)]
  [lamC    (arg : symbol) (body : ExprC)]
  [appC    (fun : ExprC) (arg : ExprC)]
  [ifC     (condition : ExprC) (yes : ExprC) (no : ExprC)]
  [boxC    (arg : ExprC)] ; define uma caixa/pacote
  [unboxC  (arg : ExprC)] ; desempacota
  [setboxC (b : ExprC) (v : ExprC)] ; empacota
  [seqC    (b1 : ExprC) (b2 : ExprC)] ; executa b1 e depois b2
  )

(define-type ExprS
  [numS    (n : number)]
  [idS     (s : symbol)]
  [plusS   (l : ExprS) (r : ExprS)]
  [multS   (l : ExprS) (r : ExprS)]
  [bminusS (l : ExprS) (r : ExprS)]
  [uminusS (e : ExprS)]
  [lamS    (arg : symbol) (body : ExprS)]
  [appS    (fun : ExprS) (arg : ExprS)]
  [ifS     (c : ExprS) (y : ExprS) (n : ExprS)]
  [boxS    (a : ExprS)]
  [unboxS  (a : ExprS)]
  [setboxS (b : ExprS) (v : ExprS)]
  [seqS    (b1 : ExprS) (b2 : ExprS)]
  )

(define-type Value
  [numV  (n : number)]
  [closV (arg : symbol) (body : ExprC) (env : Env)]
  [boxV  (l : Location)]
  )

(define-type-alias Location number)
(define-type Binding
  [bind (name : symbol) (val : Location)])

(define-type-alias Env (listof Binding))
(define mt-env empty)
(define extend-env cons)

(define-type Storage
  [cell (location : Location) (val : Value)])
(define-type-alias Store (listof Storage))
(define mt-store empty)
(define override-store cons)

(define-type Result
  [v*s (v : Value) (s : Store)])

; ===============================================

(define (desugar [as : ExprS]) : ExprC
  (type-case ExprS as
    [numS    (n) (numC n)]
    [idS     (s) (idC s)]
    
    [lamS    (a b)     (lamC a (desugar b))]
    [appS    (fun arg) (appC   (desugar fun) (desugar arg))]
    
    [plusS   (l r) (plusC (desugar l) (desugar r))]
    [multS   (l r) (multC (desugar l) (desugar r))]
    [bminusS (l r) (plusC (desugar l) (multC (numC -1) (desugar r)))]
    [uminusS (e)   (multC (numC -1)   (desugar e))]
    
    [ifS     (c y n) (ifC (desugar c) (desugar y) (desugar n))]
    
    [boxS    (a)     (boxC    (desugar a))]
    [unboxS  (a)     (unboxC  (desugar a))]
    [setboxS (b v)   (setboxC (desugar b)  (desugar v))]
    [seqS    (b1 b2) (seqC    (desugar b1) (desugar b2))]
    ))

(define (lookup [for : symbol] [env : Env]) : Location
  (cond
    [(empty? env) (error 'lookup (string-append (symbol->string for) " não foi encontrado"))] ; livre (não definida)
    [else (cond
            [(symbol=? for (bind-name (first env))) ; achou!
                           (bind-val  (first env))]
            [else (lookup for (rest env))])]))      ; procura no resto

(define (fetch [l : Location] [sto : Store]) : Value
  (cond
    [(empty? sto) (error 'fetch " posição não encontrada")]
    [else (cond
            [(= l (cell-location (first sto))) ; achou!
             (cell-val (first sto))]
            [else (fetch l (rest sto))])]))    ; procura no resto

(define new-loc
  (let ([n (box 0)])
    (lambda ()
      (begin
        (set-box! n (+ 1 (unbox n)))
        (unbox n)))))

(define (num+ [l : Value] [r : Value]) : Value
  (cond
    [(and (numV? l) (numV? r)) (numV (+ (numV-n l) (numV-n r)))]
    [else (error 'num+ "um dos argumentos não é um número")]))
(define (num* [l : Value] [r : Value]) : Value
  (cond
    [(and (numV? l) (numV? r)) (numV (* (numV-n l) (numV-n r)))]
    [else (error 'num* "um dos argumentos não é um número")]))

(define (interp [a : ExprC] [env : Env] [sto : Store]) : Result
  (type-case ExprC a
    [numC (n)     (v*s (numV n) sto)]
    [idC  (n)     (v*s (fetch (lookup n env) sto) sto)] ; busca em cascata, env e em seguida no sto
    [lamC (a b)   (v*s (closV a b env) sto)]
    [seqC (b1 b2) (type-case Result (interp b1 env sto)
                    [v*s (v-b1 s-b1) ; resultado e store retornado por b1
                         (interp b2 env s-b1)])]
    [appC (f a)
          (type-case Result (interp f env sto) ; acha a função
            [v*s (v-f s-f)
                 (type-case Result (interp a env s-f) ; argumento com sto modificado pela função
                   [v*s (v-a s-a)
                        (let ([onde (new-loc)]) ; aloca posição para o valor do argumento
                          (interp (closV-body v-f)
                                  (extend-env (bind (closV-arg v-f) onde)
                                              (closV-env v-f))
                                  (override-store (cell onde v-a) s-a)))
                        ])])]
    [plusC (l r)
           (type-case Result (interp l env sto)
             [v*s (v-l s-l)
                  (type-case Result (interp r env s-l)
                    [v*s (v-r s-r)
                         (v*s (num+ v-l v-r) s-r)])])]
    [multC (l r)
           (type-case Result (interp l env sto)
             [v*s (v-l s-l)
                  (type-case Result (interp r env s-l)
                    [v*s (v-r s-r)
                         (v*s (num* v-l v-r) s-r)])])]
    [ifC (c y n) (if (zero? (numV-n (v*s-v (interp c env sto)))) (interp y env sto) (interp n env sto))]
    [boxC (a)
          (type-case Result (interp a env sto)
            [v*s (v-a s-a)
                 (let ([onde (new-loc)])
                   (v*s (boxV onde)
                        (override-store (cell onde v-a) s-a)))])]
    [unboxC (a) (type-case Result (interp a env sto)
                  [v*s (v-a s-a)
                       (v*s
                        (fetch (boxV-l v-a) s-a) ; valor
                        s-a                      ; store
                        )])]
    [setboxC (b v) (type-case Result (interp b env sto)
                     [v*s (v-b s-b)
                          (type-case Result (interp v env s-b)
                            [v*s (v-v s-v)
                                 (v*s v-v
                                      (override-store
                                       (cell (boxV-l v-b)
                                             v-v)
                                       s-v))])])]
    ))

(define (parse [s : s-expression]) : ExprS
  (cond
    [(s-exp-number? s) (numS (s-exp->number s))]
    [(s-exp-symbol? s) (idS  (s-exp->symbol s))] ; pode ser um symbol livre nas definições de função
    [(s-exp-list? s)
     (let ([sl (s-exp->list s)])
       (case (s-exp->symbol (first sl))
         [(+) (plusS   (parse (second sl))
                       (parse (third sl)))]
         [(*) (multS   (parse (second sl))
                       (parse (third sl)))]
         [(-) (bminusS (parse (second sl))
                       (parse (third sl)))]
         [(~) (uminusS (parse (second sl)))]
         
         [(func) (lamS (s-exp->symbol (second sl))
                       (parse (third sl)))] ; definição
         [(call) (appS (parse (second sl))
                       (parse (third sl)))]
         
         [(if) (ifS (parse (second sl))
                    (parse (third sl))
                    (parse (fourth sl)))]
         
         [(-#)  (boxS    (parse (second sl)))]
         [(>#)  (unboxS  (parse (second sl)))]
         [(!#)  (setboxS (parse (second sl))
                         (parse (third sl)))]
         [(seq) (seqS    (parse (second sl))
                         (parse (third sl)))]
         
         [else (error 'parse "invalid list input")]))]
    [else (error 'parse "invalid input")]))

; facilitador
(define (interpS [s : s-expression]) (interp (desugar (parse s)) mt-env mt-store))

(test (v*s-v (interp (plusC (numC 10) (appC (lamC '_ (numC 5)) (numC 10)))
                     mt-env mt-store))
      (numV 15))

(interpS '(+ 10 (call (func x (+ x x)) 16)))

(interpS '(seq (!# (-# 2) 32) (># (-# 2) (+ (># (-# 2)) 10))))

(interpS '(call (func f (call f (-# 32))) (func x (seq (!# x (+ (># x) 10)) (># x)))))