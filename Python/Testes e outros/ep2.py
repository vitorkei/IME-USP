"""
MAC0239 - Introdução à Lógica e Verificação de Programas
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
Exercício-programa 2 - Verificação Simbólica de Modelos

Código de execução: python3 ep2.py

Notas: -Este programa não utiliza a biblioteca de BDDs do PyEDA
       -Os conjuntos de estados são armazenados em um vetor de forma que são apresntados da seguinte forma se impresso:
          [0, 1, 2, ..., N-1]
          Se um estado não pertence ao conjunto de saída, ele aparece como '-' em sua respectiva posição
          Exemplo: X = ['-', 1, 2, '-', 4, '-'], N = 6
                   X[0] = estado s_0 = '-'
                   X[1] = estado s_1 = 1
                   X[2] = estado s_2 = 2
                   X[3] = estado s_3 = '-'
                   X[4] = estado s_4 = 4
                   X[5] = estado s_5 = '-'
          Ou seja, o índice de um vetor referente aos estados de S é o próprio estado.
          Um vetor que representa um sub-conjunto de S tem o mesmo tamanho de S; a única diferença é que os estados de S que não pertencem ao sub-conjunto são representados pelo caractere - ao invés de um número

       -Há diversos comandos print comentados espalhados pelo código.
          Descomentá-los faz com que o programa imprima cada operação da fórmula de forma que se torna possível acompanhar a mudança que o conjunto de saída sofre do começo (quando ele equivale ao conjunto S) ao fim
"""

# -*- coding: utf-8 -*-
from parser import *
from pyeda.inter import *
import re
import os

"""
SAT
[OK] phi is 1
[OK] phi is 0
[OK] phi is -phi_1
[OK] phi is *(phi_1)(phi_2)
[OK] phi is +(phi_1)(phi_2)
[OK] phi is EX phi_1 => SAT_EX [OK]
[OK] phi is AX phi_1
[OK] phi is AF phi_1 => SAT_AF [OK]
[OK] phi is EG phi_1
[OK] phi is E[phi_1 U phi_2] => SAT_EU [OK]
[OK] phi is EF phi_1
[OK] phi is AG phi_1
[OK] phi is A[phi_1 U phi_2]
[OK] phi is phi_1 -> phi_2 (considera como +(-phi_1)(phi_2)
"""

def SAT(phi, S, rots, N, pares):

  # phi is T: return S
  if (str(phi) == '1'):
    return S

  # phi is F: return 0
  elif (str(phi) == '0'):
    return 0

  # phi is atomic formula: return {s belongs_to S | phi belongs_to L(s)}
  elif (re.match('x[0-9]+', str(phi))):
    s = [0 for i in range (N)]
    for i in range (0, N):
      s[i] = '-'
    for i in range (0, N):
      aux = re.findall ('(?<!-)x[0-9]+', rots[i])
      for j in range (0, len(aux)):
        if (str(phi) == aux[j]):
          s[i] = i
    print (s)
    return s

  # phi is -phi_1: return S\SAT(phi_1)
  elif (phi.kind == '-'):
    

if __name__ == '__main__':
  # Entradas do programa.
  # Pares recebe uma string, extrai os números em formato de string para um vetor e converte as strings de números em inteiros
  N = int(input('Entre com o número N de estados do modelo: '))
  S = [0 for i in range (N)]
  for i in range (0, N):
    S[i] = i

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
  aux0 = re.findall ('[0-9]+', aux)
  aux = re.findall('\([^)]*\)', aux)
  aux_M = 0
  M = 0
  for i in range (0, len(aux0)):
    aux_M = int(aux0[i])
    if (aux_M > M):
      M = aux_M
  # M é um número que indica o número de rótulos do modelo
  # M é determinado pelo "maior rótulo" (maior xn) que um estado possui. Por exemplo, os rótulos que os estados possuem forem x1, x2, x4 e x7, considera-se que há 7 átomos no modelo
  # aux_M é uma variável auxiliar para encontrar M
  # Cria uma matriz de tamanho NxM (M rótulos para N estados)
  rots_aux = [0 for i in range (N)]               # Cria um array cujos elementos são arrays da forma -x1, -x2, ..., -xM
  for i in range (N):
    rots_aux[i] = [0 for j in range (M)]
    for j in range (M):
      rots_aux[i][j] = "-x" + str(j + 1)

  rots_aux2 = [0 for i in range (N)]
  for i in range (0, N):
    rots_aux2[i] = re.findall('[0-9]+', aux[i])     # Recebe apenas a parte numérica de cada átomo
    for j in range (0, len(rots_aux2[i])):          # Converte de formato string para formato int
      rots_aux2[i][j] = int(rots_aux2[i][j])
    if (len(rots_aux2[i]) < M):
      for j in range (0, len(rots_aux2[i])):
        rots_aux[i][rots_aux2[i][j] - 1] = "x" + str(rots_aux2[i][j])

  rotulos = [0 for i in range (N)]
  for i in range (0, N):
    rotulos[i] = rots_aux[i][0]
    for j in range (1, M):
      rotulos[i] = rotulos[i] + " * " + rots_aux[i][j]

#  print (rotulos)

  # Recebe uma string com a fórmula em CTL
  # CTL é a fórmula CTL no formato CTLtree do arquivo parser.py
  CTL = input('Entre com a fórmula CTL a ser testada: ')
  CTL = CTLtree(CTL)
#  print (CTL)

  # Recebe o estado inicial k de interesse e cria um vetor de um único elemento contendo uma string referente a esse estado
  k = input('Entre com o estado k de interesse: ')

#  k = re.findall('[0-9]+', k)
#  k = int(k)

  # Recebe o conjunto de saída
  exit_set = SAT(CTL, S, rotulos, N, pares)
  # Imprime o conjunto de saída
  print ("Saída: " + str(exit_set))

"""
  # e busca, nesse conjunto, por k. Se houver, a fórmula é satisfazível.
  if (exit_set[k] != '-'):
    print ("Satisfaz")
  else:
    print ("Não satisfaz")
"""
