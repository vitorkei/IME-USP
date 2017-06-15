# -*- coding: utf-8 -*-

"""
Nome                        |   NUSP

Henrique Cerquinho          |  9793700
João Pedro Miguel de Moura  |  7971622
Tomás Paim                  |  7157602
Vítor Kei Taira Tamada      |  8516250

"""

import matplotlib.pyplot as plt
import math
g = 9.80665
m = 0.145 #Massa do celular
L = 3.84 #Comprimento do fio
eTotal = m*g*0.06 #Energia total

"""
Lê o arquivo de nome 'nome' e retorna uma tupla de listas com os valores para
plotar os gráficos, o vetor de sinal medio do respectivo experimento e o seu
offset
"""
def leArquivo(nome):
	referencias = [1.2, 1.1, 1.3, 1.25, 1.2]
	tempo, x, y, z, F, sinalMedio = [], [], [], [], [], []
	ok = 0

	arquivo = open(nome, 'r')
	valores = []

	for linha in (arquivo):

		#Remove os \n do arquivo e separa as linhas por ';'
		#Aqui serão colocadas tuplas de 5 strings para cada linha do arquivo
		#Depois converteremos os valores para float
		L = linha.replace('\n', '')
		valores.append(tuple(map(str, L.split(';'))))
	arquivo.close()

	#Retiramos a primeira linha pois ela é basicamente uma legenda para o
	#arquivo
	del valores[0]

	#Pegamos as tuplas de 'valores' e convertemos cada string para float, e
	#colocamos nas respectivas listas
	#As tuplas estao na forma (tempo, x, y, z, F)

	for i in range(len(valores)):
		tempo.append(float(valores[i][0]))
		x.append(float(valores[i][1]))
		y.append(float(valores[i][2]))
		z.append(float(valores[i][3]))
		F.append(float(valores[i][4]))

		#Aqui usamos o vetor de referencias para fazer a checagem se o valor de
		#F excede o respectivo valor desse vetor. Se exceder, ele começa a
		#armazenar os valores de F tambem em um vetor de sinal medio para
		#comparar com os outros sinais medios depois, no caso do pendulo
		if (nome.startswith('pendulo')):
			for j in range(5):
				if (nome == 'pendulo' + str(j+1) + '.csv'):
					if (ok != 0 or float(valores[i][4]) > referencias[j]):
						#Coloca o tempo que o movimento do pendulo começa em ok
						#paratermos os offsets
						if (ok == 0):
							offset = tempo[i]
							ok = 1
						sinalMedio.append(valores[i][4])

	valores = (tempo, x, y, z, F)

	if (nome.startswith('pendulo')):
		return valores, sinalMedio, offset
	else:
		return valores

"""
Recebe o tempo e devolve a altura atual do pendulo
"""
def alturaPendulo(t):
	tetha = 0.17889*math.sin(1.59807*t + 1.716)
	return (L - L*math.cos(tetha))

"""
Gera o grafico analitico do pendulo, desconsiderando fatores externos
"""
def geraPenduloAnalitico(step, tf, leituras):
	t, auxN = 0, 0
	tempo, pos, vel = [], [], [] #Listas que serao retornadas
	cronometro, auxV, tempos, mediaFinal = [], [], [], [] #Listas auxiliares

	#Preenche mediaFinal com 0
	for i in range(10):
		mediaFinal.append(0)

	#Soma todos os valores no cronometro
	for i in range(5):
		cronometro = leituras[i]
		for j in range(10):
			mediaFinal[j] += cronometro[j][0] + cronometro[j][1]

	#Tira a media dos valores somados
	for i in range(10):
		mediaFinal[i] /= 10

	#Coloca cada tempo cronometrado somado com os anteriores no vetor de tempos
	for i in range(len(cronometro)):
		auxN += mediaFinal[i]
		tempos.append(auxN)

	#Preenche um vetor para usar na plotagem e encaixar com os tempos
	for i in range(len(tempos)):
		auxV.append(0.06)

	#Gera os vetores para o gráfico da analítica
	while (t <= tf):
		h = alturaPendulo(t)
		auxH = 2*(eTotal - m*g*h)/m
		if (auxH >= 0):
			v = math.sqrt(auxH)
		else:
			v = -math.sqrt(-auxH)

		tempo.append(t)
		pos.append(h)
		vel.append(v)
		t += step

	#Faz os plots
	plt.title("Pendulo Analitico")
	plt.xlabel("tempo")
	plt.ylabel("altura")
	plt.grid(True)
	plt.plot(tempo, pos, 'b-')
	plt.plot(tempos, auxV, 'go')

	return(tempo, pos, vel)

"""
Gera o grafico analitico do experimento da rampa
"""
def geraRampa(step, tf, leituras):
	#acc é aproximadamente a gravidade*sen(angulo da rampa)
	t, acc = 0, 3.98566
	tempo, pos, vel, posCrom, posAnalit, velAnalit = [], [], [], [], [], [] #Listas que serao retornadas
	cronometro, tempos, mediaFinal = [], [], [] #Listas auxiliares
	marcacoes = [10, 20]

	#Preenche mediaFinal com 0
	for i in range(2):
		mediaFinal.append(0)

	#Soma todos os valores no cronometro
	for i in range(5):
		cronometro = leituras[i]
		for j in range(2):
			mediaFinal[j] += cronometro[j][0] + cronometro[j][1]

	#Tira a media dos valores somados
	for i in range(2):
		mediaFinal[i] /= 10

	#Gera os vetores para o gráfico da rampa analitica e de Euler
	p, pc, pa, v, va = 0, 0, 0, 0, 0
	while (t <= tf):
		#gera as posicoes de Euler e Euler Cromer
		p += v*step
		v += acc*step
		pc += v*step

		#gera as posicoes e velocidades analiticas
		pa = acc*(t**2)/2
		va = acc*t

		tempo.append(t)
		pos.append(p)
		posCrom.append(pc)
		posAnalit.append(pa)
		vel.append(v)
		velAnalit.append(va)
		t += step

	#Faz os plots
	plt.title("Rampa Euler e Analitica")
	plt.xlabel("tempo")
	plt.ylabel("posicao")
	plt.grid(True)
	Euler, = plt.plot(tempo, pos, 'b-', label = 'Euler')
	EulerCrom, = plt.plot(tempo, posCrom, 'y-', label = 'Euler Cromer')
	Analit, = plt.plot(tempo, posAnalit, 'r-', label = 'Analitica')
	plt.plot(mediaFinal, marcacoes, 'go')

	plt.legend(handles = [Euler, EulerCrom, Analit])

	return(tempo, pos, vel)

"""
Gera o gráfico do arquivo 'nome' do acelerômetro usando a funçã leArquivo
"""
def geraGrafico(nome, valores):
	for i in range(5):
		plt.subplot(321 + i)

		tempo = valores[i][0]
		x = valores[i][1]
		y = valores[i][2]
		z = valores[i][3]
		F = valores[i][4]

		# Plota os títulos
		plt.title(nome + ' ' + str(i+1))
		plt.grid(True)
		plt.ylabel("x, y, z, F")

		# Plota os gráficos e da nomes (label) para criar as legendas
		plt.plot(tempo, x, 'r-')
		plt.plot(tempo, y, 'b-')
		plt.plot(tempo, z, 'y-')
		plt.plot(tempo, F, 'k-')

	return

"""
Gera o grafico de sinal medio dos 5 experimentos do pendulo
"""
def geraSinalMedio(tempos, sinalMedio):
	Exp1,=plt.plot(tempos[0], sinalMedio[0], 'r-', alpha = 0.6, label = 'Exp1')
	Exp2,=plt.plot(tempos[1], sinalMedio[1], 'y-', alpha = 0.6, label = 'Exp2')
	Exp3,=plt.plot(tempos[2], sinalMedio[2], 'b-', alpha = 0.6, label = 'Exp3')
	Exp4,=plt.plot(tempos[3], sinalMedio[3], 'c-', alpha = 0.6, label = 'Exp4')
	Exp5,=plt.plot(tempos[4], sinalMedio[4], 'k-', alpha = 0.6, label = 'Exp5')

	plt.legend(handles = [Exp1, Exp2, Exp3, Exp4, Exp5])
	plt.title("Sinal Medio")
	plt.grid(True)
	return

"""
Le o arquivo de cronometro do experimento com o pendulo e retorna uma lista
com 5 listas, uma para cada experimento, cada uma contendo os tempos das 10
marcaçoes feitas pelas duas pessoas
"""
def leCronometroPendulo():
	cronometro = []
	leituras = []

	arquivo = open('cronometro.txt', 'r')

	for linha in (arquivo):
		# Remove os \n
		L = linha.replace('\n', '')
		# Separa os elementos por tab
		cronometro.append(tuple(map(str, L.split('\t'))))
	arquivo.close()


	#Transforma todos os valores em float e juntamos em pares, ignorando os
	#nomes "Experimento1", "Experimento2", etc

	for i in range(5):
		listaAux = []
		leituras.append(listaAux)
		for j in range(10):
			leituras[i].append((float(cronometro[(10*i) + 1 + i + j][0]),
			float(cronometro[(10*i) + 1 + i + j][1])))

	return leituras

def leCronometroRampa():
	cronometro = []
	leituras = []

	arquivo = open('rampa.txt', 'r')

	for linha in (arquivo):
		# Remove os \n
		L = linha.replace('\n', '')
		# Separa os elementos por tab
		cronometro.append(tuple(map(str, L.split('\t'))))
	arquivo.close()

	#Transforma todos os valores em float e juntamos em pares, ignorando os
	#nomes "Experimento1", "Experimento2", etc

	for i in range(5):
		listaAux = []
		leituras.append(listaAux)
		for j in range(2):
			leituras[i].append((float(cronometro[2*i + j][0]),
								float(cronometro[2*i + j][1])))

	return leituras


def main():
	leiturasP = leCronometroPendulo()
	leiturasR = leCronometroRampa()
	valoresP, valoresR = [], [] #Guarda os valores gerados pelo toolbox
	sinalMedio = []
	tempos = [[],[],[],[],[]] #Lista de tempos para plotar o sinal medio
	offsets = [] #Lista de offsets para cada experimento do pendulo

	#Le os arquivos e gera o sinal medio e o offset de cada experimento
	for i in range(5):
		nome = 'pendulo' + str(i+1) + '.csv'

		valoresP.append(leArquivo(nome)[0])
		sinalMedio.append(leArquivo(nome)[1])
		offsets.append(leArquivo(nome)[2])

		nome = 'rampa' + str(i+1) + '.csv'

		valoresR.append(leArquivo(nome))

	#Cria a lista de tempos
	for i in range(5):
		for j in range(len(sinalMedio[i])):
			tempos[i].append(j)

	#Gera os graficos do toolbox do pendulo
	plt.figure(1)
	geraGrafico('Pendulo', valoresP)

	#Gera o grafico do sinal medio do pendulo
	plt.figure(2)
	geraSinalMedio(tempos, sinalMedio)

	#Gera o grafico do pendulo analitico
	plt.figure(3)
	geraPenduloAnalitico(0.001, 50, leiturasP)

	plt.figure(4)
	geraRampa(0.001, 11, leiturasR)

	#Gera os graficos do toolbox da rampa
	plt.figure(5)
	geraGrafico('Rampa', valoresR)

	plt.show()
	return
main()
