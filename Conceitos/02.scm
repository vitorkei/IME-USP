#lang plai-typed

; "template" de uma função
(define-type FunDefC
  [fdC (name : symbol) (arg : symbol) (body : ExprC)])

(define-type Binding
  [bind (name : symbol) (val : number)])

(define-type-alias Env (listof Binding))
(define mt-env empty)
(define extend-env cons)

; Novo tipo, com funções
; Precisamos de duas novas entradas:
;   - identificador, para argumentos
;   - aplicação da função
(define-type ExprC
  [numC (n : number)]
  [idC  (s : symbol)] ; identificador
  [appC (fun : symbol) (arg : ExprC)] ; aplicação, com o nome da função
                                      ; e o valor do argumento

  [plusC (l : ExprC) (r : ExprC)]
  [multC (l : ExprC) (r : ExprC)]
  [ifC   (condition : ExprC) (yes : ExprC) (no : ExprC)])

; Açúcar sintático
(define-type ExprS
  [numS    (n : number)]
  [idS     (s : symbol)]
  [appS    (fun : symbol) (arg : ExprS)]
  [plusS   (l : ExprS) (r : ExprS)]
  [bminusS (l : ExprS) (r : ExprS)]
  [uminusS (e : ExprS)]
  [multS   (l : ExprS) (r : ExprS)]
  [ifS     (c : ExprS) (y : ExprS) (n : ExprS)])

(define (desugar [as : ExprS]) : ExprC
  (type-case ExprS as
    [numS (n) (numC n)]
    [idS  (s) (idC s)] ; este é fácil
    [appS (fun arg) (appC fun (desugar arg))]  ; fun é um symbol,
                                                 ; não precisa de desugar
    [plusS (l r) (plusC (desugar l) (desugar r))]
    [multS (l r) (multC (desugar l) (desugar r))]
    [bminusS (l r) (plusC (desugar l) (multC (numC -1) (desugar r)))]
    [uminusS (e) (multC (numC -1) (desugar e))]
    [ifS (c y n) (ifC (desugar c) (desugar y) (desugar n))]))

(define (interp [a : ExprC] [env : Env] [fds : (listof FunDefC)]) : number
  (type-case ExprC a
    [numC (n) n]
    [idC  (n) (lookup n env)]
    ; Aplicação de função é que precisa de subst
    [appC (f a)
          (local ([define fd (get-fundef f fds)]) ;pega a def em fd
            (interp (fdC-body fd) ; expresão
                    (extend-env
                        (bind (fdC-arg fd) (interp a env fds))
                        env) ; novo environment
                    fds))]
    [plusC (l r) (+ (interp l env fds) (interp r env fds))]
    [multC (l r) (* (interp l env fds) (interp r env fds))]
    [ifC (c y n) (if (zero? (interp c env fds)) (interp y env fds) (interp n env fds))]))

(define (get-fundef [n : symbol] [fds : (listof FunDefC)]) : FunDefC
  (cond
    [(empty? fds) (error 'get-fundef "referência para função não definida")]
    [(cons? fds) (cond
                   [(equal? n (fdC-name (first fds))) (first fds)] ; achou!
                   [else (get-fundef n (rest fds))] ; procura no resto
                   )]))

(define (lookup [for : symbol] [env : Env]) : number
  (cond
    [(empty? env) (error 'lookup "name not found")]
    [else (cond
            [(symbol=? for (bind-name (first env)))
                           (bind-val  (first env))]
            [else (lookup for (rest env))])]))

; o parser precisa tratas de chamadas
(define (parse [s : s-expression]) : ExprS
  (cond
    [(s-exp-number? s) (numS (s-exp->number s))]
    [(s-exp-list? s)
     (let ([sl (s-exp->list s)])
       (case (s-exp->symbol (first sl))
         [(+) (plusS (parse (second sl)) (parse (third sl)))]
         [(*) (multS (parse (second sl)) (parse (third sl)))]
         [(-) (bminusS (parse (second sl)) (parse (third sl)))]
         [(~) (uminusS (parse (second sl)))]
         [(call) (appS (s-exp->symbol (second sl)) (parse (third sl)))]
         [(if) (ifS (parse (second sl)) (parse (third sl)) (parse (fourth sl)))]
         [else (error 'parse "invalid list input")]))]
    [else (error 'parse "invalid input")]))

(define biblioteca
  (list
   [fdC 'dobro 'x (plusC (idC 'x) (idC 'x))]
   [fdC 'quadrado 'y (multC (idC 'y) (idC 'y))]
   [fdC 'fatorial 'n
        (ifC (idC 'n)
             (numC 1)
             (multC (appC 'fatorial (plusC (idC 'n) (numC -1)))
                    (idC 'n)))]
   [fdC 'narciso 'narciso (multC (idC 'narciso) (numC 1000))]))

(interp (desugar (parse '(+ -1400 (call fatorial 7)))) mt-env biblioteca)
(interp (desugar (parse '(call dobro (+ 5 1)))) mt-env biblioteca)
(interp (desugar (parse '(call fatorial 7))) mt-env biblioteca)
(test
 (interp (desugar (parse '(call narciso (call fatorial 7)))) mt-env biblioteca)
 5040000)

(interp (appC 'f1 (numC 3)) mt-env
        (list
         [fdC 'f1 'x (appC 'f2 (numC 4))]
         [fdC 'f2 'y (plusC (idC 'x) (idC 'y))]))
(interp (desugar (parse '(call fatorial 3)))
        mt-env
        biblioteca)