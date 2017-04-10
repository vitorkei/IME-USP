"""
MAC0239 - Introdução à Lógica e Verificação de Programas
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
Exercício-programa 2 - Verificação Simbólica de Modelos

Código de execução: python3 ep2.py

Notas:
"""

# -*- coding: utf-8 -*-
from pyeda.inter import *
from parser import *
import re
import os

"""
********** Notação para este EP **********
> Constantes True e False = 1 e 0 respectivamente

> Átomos (p, q, r, ...) = x_0, x_1, x_2, ...

> Operadores lógicos:
>>  E = *
>>  OU = +
>>  NÃO = -
>>  PyEDA utiliza &, | e ~ no lugar de *, + e - respectivamente (E, OU, NÃO)

> Operadores temporais:
>>  1a letra: A ou E
>>  2a letra: X, F, G ou U

> Notação prefixa
>>  Exemplo: +(phi_1)(phi_2) ao invés de phi_1 + phi_2
>>  Exemplo: EU(1)(phi_1) ao invés de E[1 U phi_1]
>> Operador unário não utiliza parentêses
>>>   Exemplo: "-x_3", "AX EG x_2"

> Representação fatorada de um estados
>> Se x_i pertence a s, é escrito como x_i
>> Se x_i não pertence a s, é escrito como -x_i
>> Exemplo: m = 3, L(s) = {x_1, x_3}. s é dado por x_1 * -x_2 * x_3

> Representação fatorada de um conjunto de estados
>> X é um conjunto de estados. X é representado por uma disjunção da fórmula de cada estado
>> B_X = B_s1 + B_s2 + ...

> Representação fatorada do modelo
>> A representação fatorada de uma aresta s->s' é dada pela conjunção entre as fórmulas de s e de s'
>> A representação fatorada completa para a relação de transição B_-> é a disjunção das fórmulas de cada aresta

> Função de pré-imagem fraca simbólica
>> Fazer conjunção B_X' * B_->
>> Usar a operação Exists (aula de BDDs) para "eliminar" todas as variáveis "linha"
>>>  Pre_E(X) = Exists (x_i')(B_X' * B_->), i = 1, ..., m
"""
# phi é a fórmula CTL
# S é o conjunto que contém todos os estados do modelo
# rots = rotulos
# N é o número de estados que o modelo possui
def SAT (phi, S, rots, N):

  # phi is T: return S
  if (str(phi) == '1'):
    return S

  # phi is F: return 0
  elif (str(phi) == '0'):
    return 0

  # phi is atomic formula: return {s belongs_to S | phi belongs_to L(s)}
  elif (re.match ('"?x[0-9]+"?', str(phi))):
    s_1 = [0 for x in range (N)]
    for x in range (0, N):
      s_1[x] = '-'
    aux = 0
    for x in range (0, len(rots)):
      for y in range (0, len(rots[x])):
        if (rots[x][y] == str(phi)):
          s_1[aux] = x
      aux = aux + 1
    return s_1

  # phi is ~phi_1: return S\SAT(phi_1)
  elif (re.match ('-.*', str(phi))):            # Verifica se phi é, especificamente, da forma -phi_1
    aux = re.findall ('(?<=-).*', str(phi))     # Separa phi_1
    aux = str(aux[0])                           # A linha de cima cria um vetor. Esta transforma em string
    aux = CTLtree(aux)                          # transforma a string em uma CTL
    s_1 = SAT(aux, S, rots, N)                  # faz SAT(phi_1)
    for x in range (0, N):                      # S\SAT(phi_1)
      if (s_1[x] != '-'):
    return S

  # phi is phi_1 ^ phi_2: return SAT(phi_1) ^ SAT(phi_2)
  elif (re.match('\*.*', str(phi))):            # Verifica se phi é, especificamente, da forma *(phi_1)(phi_2)
    aux = re.findall ('(?<=\*).*', str(phi))    # Separa (phi_1)(phi_2)
# separa phi_1 de phi_2
    aux = str(aux[0])
    aux = CTLtree(aux)
    s_1 = SAT(aux, S, rots, N)
    
"""    
def SAT_AF (phi, S):
  X = S
  Y = SAT (phi, S)
  while (X != Y):
    X = Y
    # Y = Y + {conjunto de s tal que para todo s' com s->s', s' pertence a Y}   # Pré-imagem forte
  return Y

def SAT_EU (phi, psi, S):
  W = SAT (phi, S)
  X = S
  Y = SAT (psi, S)
  while (X != Y):
    X = Y
    # Y = Y + (W * {conjunto de s tal que existe s' tal que s->s' e s' pertence a Y})   # Pré-imagem fraca
  return Y

# Devolve a pré-imagem fraca de X
def SAT_EX (phi, S):
  X = SAT (phi, S)
  Y = {conjunto de s_0 tal que s_0->s_1 para algum s_1 pertencente a X}   # Pré-imagem fraca
  return Y
"""

if __name__ == '__main__':
  # Entradas do programa.
  # Pares recebe uma string, extrai os números em formato de string para um vetor e converte as strings de números em inteiros
  N = int (input ('Entre com o número N de estados do modelo: '))
  S = [0 for x in range (N)]
  for x in range (0, N):
    S[x] = x

  # Cria um array em que cada elemento é o par e estados que compõe uma aresta
  # Formato: (i,j) == i->j
  pares = input ('Entre com a lista de pares-arestas do modelo de Kripke: ')
  pares = re.findall ('\([0-9]+,[0-9]+\)', pares)
  print (pares)

  # Cria um array auxiliar em que cada elemento é o rótulo (conjunto de átomos) refernte a cada estado
  # Leitura mais clara da regex:   \( [^)]* \)   Um rótulo qualquer
  aux = input ('Entre com a lista de rótulos dos N estados: ')
  aux = re.findall ('\([^)]*\)', aux)
  print (aux)
  # Cria um array de arrays (de tamanhos variados) em que cada rotulos[i][j] contém um átomo
  rotulos = [0 for x in range (N)]
  for x in range (0, N):
    rotulos[x] = re.findall ('x[0-9]+', aux[x])
  print (rotulos)

  # Recebe uma string com a fórmula em CTL
  # CTL é a fórmula CTL no formato CTLtree do arquivo parser.py
  CTL = input ('Entre com a fórmula CTL a ser testada: ')
  CTL = CTLtree(CTL)
  answer = str(CTL)
  print (answer)

  # Recebe o estado inicial k de interesse e cria um vetor de um único elemento contendo uma string referente a esse estado
  k = input ('Entre com o estado k de interesse: ')
  k = re.findall ('x[0-9]+', k)

  print ("Saída: " + str(SAT(CTL, S, rotulos, N)))

"""
  a = [0 for x in range (6)]
  for x in range (0, 6):
    a[x] = '-'
  a[0] = 'o'
  a[1] = 'i'
  a[2] = 3
  a[2] = a[2] + 1
  a[3] = 'batata'
  for x in range(0, 6):
    print (a[x])
"""
