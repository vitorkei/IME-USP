"""
MAC0239 - Introdução à Lógica e Verificação de Programas
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
Exercício-programa 2 - Verificação Simbólica de Modelos

Código de execução: python3 ep2.py

Notas: -Este programa não utiliza a biblioteca de BDDs do PyEDA

       -Este programa trabalha com os estados da seguinte forma:
          Os conjuntos de estados são armazenados em um vetor de forma que são apresentados da seguinte forma se impresso:
          [0, 1, 2, ..., N-1]
          Se um estado não pertence ao conjunto de saída, ele aparece como o caracter - em sua respectiva posição
          Exemplo: X = ['-', 1, 2, '-', 4, '-'], N = 6
                   X[0] = estado s_0 = -
                   X[1] = estado s_1 = 1
                   X[2] = estado s_2 = 2
                   X[3] = estado s_3 = -
                   X[4] = estado s_4 = 4
                   X[5] = estado s_5 = -
          Ou seja, o índice de um vetor referente aos estados de S é também o número referente ao próprio estado segundo a notação deste programa.
          Um vetor que representa um sub-conjunto de S tem o mesmo tamanho de S; a única diferença é que os estados de S que não pertencem ao sub-conjunto são representados pelo caracter - ao invés de um número (como é possível ver acima)

       -Os rótulos operam de uma maneira semelhante aos estados: os rótulos de cada estado estão armazenados em um array cujo índice corresponde ao estado.
          Ou seja, os rótulos do estado 0 encontram-se no elemento de índice 0 do vetor dos rótulos; os rótulos do estado 1 estão no elemento de índice 1 do vetor dos rótulos; e assim por diante.
          Cada elemento desse array que armazena os rótulos é um outro array.
          Ou seja, rotulos[0][0] possui um rótulo do estado 0, rotulos[0][1] possui outro e assim por diante.
          Como os estados possuem números variados de rótulos, rotulos[0] pode ter um tamanho diferente de rotulos[1] e assim por diante.
          Isso ocorre porque rotulos[i] armazena apenas os rótulos que o estado possui.

       -Há diversos comandos print comentados espalhados pelo código (com # no começo da linha e não identado como os comentários)
          Descomentá-los faz com que o programa imprima cada operação da fórmula e seu respectivo conjunto de forma que se torna possível acompanhar a mudança que o conjunto de saída sofre do começo (quando ele equivale ao conjunto S) ao fim, ou como o programa armazena as informações inseridas pelo usuário (estados, rótulos, pares origem-destino de transição de estados...) no main do programa
"""

# -*- coding: utf-8 -*-
from parser import *
import re
import os

# SAT_EU(phi, psi)
def SAT_EU (phi, psi, S, rots, N, pares):
  W = SAT(phi, S, rots, N, pares)           # W := SAT(phi)
  X = [0 for i in range (N)]                # X := S
  for i in range (0, N):
    X[i] = S[i]
  Y = SAT(psi, S, rots, N, pares)           # Y := SAT(psi)
  flag0 = False
  for i in range (0, N):                    # Verifica se X != Y
    if (X[i] != Y[i]):
      flag0 = True

  while (flag0):
    #=======================================
    for i in range (0, N):                  # X := Y
      X[i] = Y[i]
    #=======================================
    S_1 = [0 for i in range (N)]            # {s pertence a S | existe s' tal que s->s' e s' pertence a Y}
    for i in range (0, N):
      S_1[i] = '-'
    for i in range (0, len(pares)):         # Constrói a pré-imagem fraca de Y (S_1)
      if (Y[pares[i][1]] != '-'):
        S_1[pares[i][0]] = pares[i][0]
#    print ("S_1: " + str(S_1))

                                            # W ^ S_1
    for i in range (0, N):                  # W[i] == '-' & S_1[i] == '-' => (entra no if) S_1[i] = '-'
      if (W[i] == '-'):                     # W[i] == '-' & S_1[i] != '-' => (entra no if) S_1[i] = '-'
        S_1[i] = '-'                        # W[i] != '-' & S_1[i] == '-' => (não entra no if) S_1[i] == '-' (sem alteração)
                                            # W[i] != '-' & S_1[i] != '-' => (não entra no if) S_1[i] != '-' (sem alteração)
#    print ("W&S: " + str(S_1))
                                            # Y v ((W ^ S_1) = W&S)
    for i in range (0, N):                  # Y[i] == '-' & W&S == '-' => (não entra no if) Y[i] == '-' (sem alteração)
      if (S_1[i] != '-' or Y[i] != '-'):    # Y[i] == '-' & W&S != '-' => (entra no if) Y[i] = i
        Y[i] = i                            # Y[i] != '-' & W&S == '-' => (entra no if) Y[i] = i
                                            # Y[i] != '-' & W&S != '-' => (entra no if) Y[i] = i
#    print ("Y U W&S: " + str(Y))
    #=======================================
    flag0 = False                           # Verifica se X == Y
    for i in range (0, N):
      if (X[i] != Y[i]):
        flag0 = True

  return Y

# SAT_EX(phi)
def SAT_EX (phi, S, rots, N, pares):
  X = SAT (phi, S, rots, N, pares)    # Faz SAT(X)
  Y = [0 for i in range (N)]          # Cria o array (conjunto) Y que será a pré-imagem fraca de X
  for i in range (0, N):              # Marca todos os elementos de Y com -, ou seja, o conjunto está inicialmente vazio
    Y[i] = '-'                        #   e será preenchido com a pré-imagem fraca de X
  for i in range (0, len(pares)):     # Encontra a pré-imagem fraca de X
    if (X[pares[i][1]] != '-'):
      Y[pares[i][0]] = pares[i][0]
  return Y

# SAT_AF(phi)
def SAT_AF (phi, S, rots, N, pares):
  X = [0 for i in range (N)]                        # X := S
  for i in range (0, N):
    X[i] = S[i]
  Y = SAT(phi, S, rots, N, pares)                   # Y := SAT(phi)
  flag0 = False
  for i in range (0, N):                            # Verifica se X != Y
    if (X[i] != Y[i]):
      flag0 = True

  while (flag0):                                    # "repeat until X = Y"
    # ==============================================
    for i in range (0, N):                          # X := Y
      X[i] = Y[i]
    # ==============================================
    S_1 = [0 for i in range (N)]                    # S_1 = {s pertence a S | se s->s', então s' pertence a Y}
    for i in range (0, N):                          # Inicia S_1 como um "conjunto vazio" (todos os seus elementos são o caracter - )
      S_1[i] = '-'
    for i in range (0, N):                          # Construção do conjunto S_1
      flag1 = True                                  # Impede que um mesmo estado s_i seja verificado se faz parte da pré-imagem forte
      for j in range (0, len(pares)):               # Verifica cada transição (s_0->s_1) / par (i,j)
        if (pares[j][0] == i and flag1):            # Verifica se a origem do par que está sendo visto é o estado que será analisado
          if (Y[pares[j][1]] == '-'):               # Verifica se o destino do par que está sendo visto pertence a Y
            S_1[i] = '-'                            # Uma vez que um destino não está em Y, sabe-se que o estado não faz parte
            flag1 = False                           #   da pré-imagem forte
          elif (Y[pares[j][1]] != '-' and flag1):
            S_1[i] = i
#    print ("Pré-imagem forte:" + str(S_1))
    # ==============================================
    for i in range (0, N):                          # Y := Y v S_1
      if (Y[i] == '-' and S_1[i] != '-'):
        Y[i] = i
    # ==============================================
    flag0 = False                                   # Verifica se X == Y
    for i in range (0, N):
      if (X[i] != Y[i]):
        flag0 = True

  return Y

# SAT(phi)
# phi é a fórmula CTL no formato CTLtree
# S é o conjunto que contém todos os estados do modelo
# rots = rotulos
# N é o número de estados que o modelo possui
# pares é o array de arrays que contém o par origem-destino de uma transição de estados
def SAT(phi, S, rots, N, pares):

  # phi is T: return S
  if (str(phi) == '1'):
    return S

  # phi is F: return 0
  elif (str(phi) == '0'):
    return 0

  # phi is atomic formula: return {s belongs_to S | phi belongs_to L(s)}
  elif (re.match('"?x[0-9]+"?', str(phi))):   # Verifica se phi é um átomo. Consegue ler x[0-9]+ e "x[0-9]+"
    s_1 = [0 for i in range (N)]              # Cria um array s_1 que será o conjunto de saída
    for i in range (0, N):                    # Marca todos os elementos de s_1 com '-'
      s_1[i] = '-'                            #   para indicar quais elementos não pertencem ao conjunto
    aux = 0                                   # Auxiliar para percorrer o vetor s_1
    for i in range (0, len(rots)):            # Verifica quais estados possuem o átomo phi
      for j in range (0, len(rots[i])):       # Marca os estados pertencentes ao conjunto com seus respectivos números
        if (rots[i][j] == str(phi)):
          s_1[aux] = i
      aux = aux + 1
#    print (str(phi) + ": " + str(s_1))
    return s_1

  # phi is ~phi_1: return S\SAT(phi_1)
  elif (phi.kind == '-'):                         # Verifica se phi é da forma -phi_1
    aux = SAT(phi.childs[0], S, rots, N, pares)   # Faz SAT(phi_1)
    s_1 = [0 for i in range (N)]
    for i in range (0, N):
      s_1[i] = S[i]
    for i in range (0, N):                        # Todos os elementos de S que também pertencem a s_1 são marcados com -
      if (aux[i] != '-'):                         #   realizando S\s_1
        s_1[i] = '-'
#    print (str(phi) + ": " + str(s_1))
    return s_1

  # phi is phi_1 ^ phi_2: return SAT(phi_1) ^ SAT(phi_2)
  elif (phi.kind == '*'):                         # Verifica se phi é da forma *(phi_1)(phi_2)
    s_1 = SAT(phi.childs[0], S, rots, N, pares)   # Faz SAT(phi_1)
    s_2 = SAT(phi.childs[1], S, rots, N, pares)   # Faz SAT(phi_2)
    S_aux = [0 for i in range (N)]
    for i in range (0, N):
      S_aux[i] = S[i]
    for i in range (0, N):                        # Faz a intersecção de SAT(phi_1) com SAT(phi_2)
      if (s_1[i] == '-' or s_2[i] == '-'):
        S_aux[i] = '-'
#    print (str(phi) + ": " + str(S_aux))
    return S_aux

  # phi is phi_1 v phi_2: return SAT(phi_1) v SAT(phi_2)
  elif (phi.kind == '+'):                         # Verifica se phi é da forma +(phi_1)(phi_2)
    s_1 = SAT(phi.childs[0], S, rots, N, pares)   # Faz SAT(phi_1)
    s_2 = SAT(phi.childs[1], S, rots, N, pares)   # Faz SAT(phi_2)

    s_3 = [0 for i in range (N)]                  # Vetor (conjunto) em que todos os elementos são -
    for i in range (0, N):                        #   de tal forma que ele será preenchida com a união entre
      s_3[i] = '-'                                #   SAT(phi_1) e SAT(phi_2)

    for i in range (0, N):                        # Faz a união de SAT(phi_1) com SAT(phi_2)
      if (s_1[i] != '-' or s_2[i] != '-'):
        s_3[i] = i
#    print (str(phi) + ": " + str(s_3))
    return s_3

  # phi is AX phi_1: return SAT (-EX-phi_1)
  elif (phi.kind == 'AX'):                # Verifica se phi é da forma AX phi_1
    phi_1 = "-EX-" + str(phi.childs[0])   # Recebe -EX-phi_1 (como uma string)
    phi_1 = CTLtree(phi_1)                # Transforma phi_1 em uma variável do tipo CTLtree
    s_1 = SAT(phi_1, S, rots, N, pares)   # Faz SAT(-EX-phi_1)
#    print (str(phi) + ": " + str(s_1))
    return s_1

  # phi is EX phi_1: return SAT_EX(phi_1)
  elif (phi.kind == 'EX'):                            # Verifica se phi é da forma EX phi_1
    s_1 = SAT_EX(phi.childs[0], S, rots, N, pares)    # Faz SAT_EX(phi_1)
#    print (str(phi) + ": " + str(s_1))
    return s_1

  # phi is AF phi_1: return SAT_AF(phi_1)
  elif (phi.kind == 'AF'):                            # Verifica se phi é da forma AF phi_1
    s_1 = SAT_AF(phi.childs[0], S, rots, N, pares)    # Faz SAT_AF(phi_1)
#    print (str(phi) + ": " + str(s_1))
    return s_1

  # phi is EG phi_1: return SAT(-AF-phi_1)
  elif (phi.kind == 'EG'):                # Verifica se phi é da forma EG phi_1
    phi_1 = "-AF-" + str(phi.childs[0])   # Recebe -AF-phi_1 (como uma string
    phi_1 = CTLtree(phi_1)                # Transforma phi_1 em uma variável do tipo CTLtree
    s_1 = SAT(phi_1, S, rots, N, pares)   # Faz SAT(-AF-phi_1)
#    print (str(phi) + ": " + str(s_1))
    return s_1

  # phi is E[phi_1 U phi_2)]: return SAT_EU(phi_1, phi_2)
  elif (phi.kind == 'EU'):                                            # Verifica se phi é da forma EU(phi_1)(phi_2)
    s_1 = SAT_EU (phi.childs[0], phi.childs[1], S, rots, N, pares)    # Faz SAT_EU(phi_1, phi_2)
#    print (str(phi) + ": " + str(s_1))
    return s_1

  #phi is EF phi_1: return SAT (E[T U phi_1])
  elif (phi.kind == 'EF'):                                          # Verifica se phi é da forma EF phi_1
    s_1 = SAT_EU (CTLtree('1'), phi.childs[0], S, rots, N, pares)   # Faz SAT_EU(1, phi_1)
#    print (str(phi) + ": " + str(s_1))
    return (s_1)

  # phi is AG phi_1: return SAT (-EF-phi_1)
  elif (phi.kind == 'AG'):                  # Verifica se phi é da forma AG phi_1
    phi_1 = "-EF-" + str(phi.childs[0])     # Recebe -EF-phi_1
    phi_1 = CTLtree(phi_1)                  # Transforma phi_1 em uma variável do tipo CTLtree
    s_1 = SAT(phi_1, S, rots, N, pares)     # Faz SAT(-EF-phi_1)
#    print (str(phi) + ": " + str(s_1))
    return s_1

  # phi is A[phi_1 U phi_2]: return SAT(-(E[-phi_1 U (-phi_1 ^ -phi_2)] v EG-phi_2))
  elif (phi.kind == 'AU'):
    phi_1 = "*(-" + str(phi.childs[0]) + ")(-" + str(phi.childs[1]) + ")"   # -phi_1 ^ -phi_2
    phi_1 = "EU(-" + str(phi.childs[1]) + ")(" + phi_1 + ")"                # E[-phi_2 U (-phi_1 ^ -phi_2)]
    phi_1 = "+(" + phi_1 + ")(EG-" + str(phi.childs[1]) + ")"               # (E[-phi_2 U (-phi_1 ^ -phi_2)]) v (EG-phi_2)
    phi_1 = "-" + phi_1                                                     # -(E[-phi_2 U (-phi_1 ^ -phi_2)] v EG-phi_2)
    phi_1 = CTLtree(phi_1)                                                  # Transforma phi_1 em uma variável do tipo CTLtree
    s_1 = SAT(phi_1, S, rots, N, pares)                                     # Faz SAT(-(E[-phi_2 U (-phi_1 ^ -phi_2)] v EG-phi_2))
#    print (str(phi) + ": " + str(s_1))
    return s_1

if __name__ == '__main__':
  # Entradas do programa.
  # Pares recebe uma string, extrai os números em formato de string para um vetor e converte as strings de números em inteiros
  N = int(input('Entre com o número N de estados do modelo: '))
  S = [0 for i in range (N)]
  for i in range (0, N):
    S[i] = i
#  print (S)

  # Cria um array em que cada elemento é o par e estados que compõe uma aresta
  # Formato: (i,j) == i->j
  aux_par = input('Entre com a lista de pares-arestas do modelo de Kripke: ')
  aux_par = re.findall('\([0-9]+,[0-9]+\)', aux_par)
  # Cria um array de arrays (de tamanho) em que cada par (pares[i][0],pares[i][1]) indica a direção de mudança de estado
  # O número de pares pode variar, mas cada par tem sempre 2 elementos apenas
  pares = [0 for i in range (len(aux_par))]
  for i in range (0, len(aux_par)):
    pares[i] = re.findall('[0-9]+', aux_par[i])
    pares[i][0] = int(pares[i][0])
    pares[i][1] = int(pares[i][1])
#  print (pares)

  # Cria um array auxiliar em que cada elemento é o rótulo (conjunto de átomos) refernte a cada estado
  # Leitura mais clara da regex:   \( [^)]* \)   Um rótulo qualquer
  aux = input('Entre com a lista de rótulos dos N estados: ')
  aux = re.findall('\([^)]*\)', aux)
  # Cria um array de arrays (de tamanhos variados) em que cada rotulos[i][j] contém um átomo. Como há N estados, há N rótulos
  rotulos = [0 for i in range (N)]
  for i in range (0, N):
    rotulos[i] = re.findall('x[0-9]+', aux[i])
#  print (rotulos)

  # Recebe uma string com a fórmula em CTL
  # CTL é a fórmula CTL no formato CTLtree do arquivo parser.py
  CTL = input('Entre com a fórmula CTL a ser testada: ')
  CTL = CTLtree(CTL)
#  print (CTL)

  # Recebe o estado inicial k de interesse e cria um vetor de um único elemento contendo uma string referente a esse estado
  k = int(input('Entre com o estado k de interesse. k deve ser um número inteiro: '))
  if (k >= N):
    print ("\nk = " + str(k) + " não é um estado pertencente ao conjunto S.\nFinalizando programa...\n")
    quit()

  # Recebe o conjunto de saída
  state_set = SAT(CTL, S, rotulos, N, pares)

  # Esta é uma maneira alternativa de imprimir os estados que satisfazem a fórmula. Ele segue o formato descrito no cabeçalho do código
#  print ("\nConjuntos que satisfazem a fórmula CTL: " + str(state_set))

  # Imprime o conjunto de saída
  # Os números que aparecem são os que satisfazem a fórmula. Os que estão com o caracter - no lugar são os que não satisfazem
  print ("\nEstados que satisfazem a fórmula CTL: ", end = "")
  for i in range (N):
    if (state_set[i] != '-'):
      print (str(state_set[i]) + " ", end = "")
  print ()

  # Busca por k no conjunto de saída. Se houver, a fórmula é satisfazível.
  if (state_set[k] != '-'):
    print ("Satisfaz\n")
  else:
    print ("Não satisfaz\n")

