#lang plai-typed

(define-type ExprC
  [numC  (n : number)]
  [varC  (s : symbol)] ; não é mais idC, um identificador
  [plusC (l : ExprC)         (r : ExprC)]
  [multC (l : ExprC)         (r : ExprC)]
  [lamC  (arg : symbol)      (body : ExprC)]
  [appC  (fun : ExprC)       (arg : ExprC)]
  [ifC   (condition : ExprC) (yes : ExprC)  (no : ExprC)]
  [setC  (var : symbol)      (arg : ExprC)] ; atribuição
  [seqC  (b1 : ExprC)        (b2 : ExprC)] ; executa b1 e, depois, b2
  )

(define-type Value
  [numV  (n : number)]
  [closV (arg : symbol) (body : ExprC) (env : Env)])
