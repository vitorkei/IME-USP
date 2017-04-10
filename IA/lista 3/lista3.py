def valueIteration(mdp, e):
  S = mdp.s() # Conjunto de estados
  A = mdp.a() # Conjunto de ações
  R = mdp.r() # Conjunto de recompensas
  g = mdp.g() # Desconto gama
  # mdp.P(fs, s, a) = P(s'|s, a)
  
  U = {} # Vetor de utilidades atual (U)
  Uf = {} # Vetor de utilidades futuro (U')
  delta = 0  # Mudança máxima da utilidade de qualquer estado em uma iteração
  U_out = [] # Vetor que conterá o vetor de utilidades para cada valor de r
  
  R11 = [-1000, -3, 0] # Valores que R(1, 1) pode ter
  
  # Inicializa a utilidade de cada estado de S como zero
  for s in S:
    U[s] = 0
    Uf[s] = 0
    
  for rew in R11:
    R['(1, 1)'] = rew
    while delta < e * (1 - g) / g:
      Uf = U.copy() # U <- U'
      delta = 0 # delta <- 0
      for s in S:
        sp = 0 # somatória de probabilidade
        for a in A[s]: # max(sum(P(s'|s, a)U(s')))
          ue = 0 # utilidade esperada
          for fs in S: # fs == future state == s'
            ue += mdp.P(fs, s, a) * U[fs]
          if ue > sp:
            sp = ue
        sp *= g # gama * max(sum(P(s'|s, a)*U(s')))
        Uf[s] = R[s] + sp
        if abs(Uf[s] - U[s]) > delta:
          delta = abs(Uf[s] - U[s])
    U_out.append(U)
    
  return U_out
