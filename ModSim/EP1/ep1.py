######################################################
# Modelagem e Simulação - EP1                        #
#                                                    #
# Henrique Cerquinho                    NUSP 9793700 #
# João Pedro Miguel de Moura            NUSP 7971622 #
# Tomás Bezerra Marcondes Paim          NUSP 7157602 #
# Vítor Kei Taira Tamada                NUSP 8516250 #
######################################################

import matplotlib.pyplot as plt
import re

# Lê o arquivo de nome 'nome' e retorna uma tupla de listas com os valores para
# plotar os gráficos
def leArquivo(nome):
    tempo = []
    x = []
    y = []
    z = []
    F = []

    arquivo = open(nome, 'r')
    valores = []

    for linha in (arquivo):
        # Remove os \n do arquivo e separa as linhas por ';'
        # Aqui serão colocadas tuplas de 5 strings para cada linha do arquivo,
        # depois converteremos os valores para float
        L = linha.replace('\n', '')
        valores.append(tuple(map(str, L.split(';'))))
    arquivo.close()

    # Retiramos a primeira linha pois ela é basicamente uma legenda para o arquivo
    del valores[0]

    # Pegamos as tuplas de 'valores' e convertemos cada string para float,
    # e colocamos nas respectivas listas
    # As tuplas estão na forma (tempo, x, y, z, F)
    for i in range(len(valores)):
        tempo.append(float(valores[i][0]))
        x.append(float(valores[i][1]))
        y.append(float(valores[i][2]))
        z.append(float(valores[i][3]))
        F.append(float(valores[i][4]))
    valores = (tempo, x, y, z, F)

    return valores

# Abre o arquivo com os valores cronometrados e retorna um dicionário com os
# tempos em cada ponto (10m, 20m, 30m) e os nomes como índice
def leCronometro():
    cronometro = []
    leituras = {}

    arquivo = open('cronometro.txt', 'r')

    for linha in (arquivo):
        # Remove os '\n's
        L = linha.replace('\n', '')
        # Separa os elementos por tab
        cronometro.append(tuple(map(str, L.split('\t'))))
    arquivo.close()

    # Transforma todos os valores em float e os nomes em índices do dicionário
    # Aqui usamos 4 como passo pois a cada 4 posicoes temos um nome no arquivo
    # do cronômetro, então não podemos transformar em float
    for i in range(0, len(cronometro), 4):
        leituras[cronometro[i][0]] = [(float(cronometro[i+1][0]), float(cronometro[i+1][1])),
        (float(cronometro[i+2][0]), float(cronometro[i+2][1])),
        (float(cronometro[i+3][0]), float(cronometro[i+3][1]))]

    return leituras

# Recebe um nome e procura todas as aparições dele no dicionário e retorna a
# velocidade média dos tres experimentos
def velMedia(nome, leituras):
    v1 = 0
    v2 = 0
    v3 = 0
    regex = re.escape(nome + r"MRU")
    for key in leituras:
        if(re.search(regex, key)):
            v1 += leituras[key][0][0] + leituras[key][0][1]
            v2 += leituras[key][1][0] + leituras[key][1][1]
            v3 += leituras[key][2][0] + leituras[key][2][1]
    v1 = 10 / (v1/6)
    v2 = 20 / (v2/6)
    v3 = 30 / (v3/6)

    return ((v1 + v2 + v3) / 3)

# Recebe a lista de valores e determina o primeiro ponto em que F é maior que 1.2
# e o último ponto em que F é maior que 1.45 sendo esses os pontos de início e de
# fim do movimento, de acordo com o acelerômetro

# Escolhemos esses numeros pois eles provaram ser os que gerarm graficos mais
# fieis ao movimento realmente executado
def IniFim(valores):
    F = valores[4]
    ini = 0
    fim = 0

    for i in range(1000, len(F)):
        if (F[i] > 1.2 and ini == 0):
            ini = i
        if (F[i] >= 1.45):
            fim = i

    return ini, fim

# Recebe o tempo, a velocidade e o período do movimento e retorna o plot do
# gráfico da posição em relação ao tempo (para gerar a legenda depois)
def geraPosicaoMRU(valores, v, ini, fim):
    tempo = valores[0]
    x = []

    for i in range(len(tempo)):
        # Se for antes do início, ainda não comecou a andar
        if(i < ini):
            x.append(0)
        # Começou a andar
        elif(i <= fim):
            x.append((tempo[i] - tempo[ini]) * v)
        # Parou
        else:
            x.append(x[i - 1])

    return plt.plot(tempo, x, 'm-', linewidth = 3, alpha = 0.5, label = 'Posição')

# Versão da função acima para MUV
# Recebe o tempo, a velocidade inicial, a aceleração e o período do movimento e 
# retorna o plot do gráfico da posição em relação ao tempo
# (para gerar a legenda depois)
def geraPosicaoMUV(valores, v0, acc, ini, fim):
    tempo = valores[0]
    x = []

    for i in range(len(tempo)):
        # Se for antes do inicio, ainda não começou a andar
        if(i < ini):
            x.append(0)
        # Comecou a andar
        elif(i <= fim):
            t = tempo[i] - tempo[ini]
            x.append(t * v0 + (t**2)*acc/2)
        # Parou
        else:
            x.append(x[i - 1])

    return plt.plot(tempo, x, 'm-', linewidth = 3, alpha = 0.5, label = 'Posição')

# Versão da função acima para a velocidade
# Recebe o tempo, a velocidade inicial, a aceleração e o período do movimento e 
# retorna o plot do gráfico da velocidade em relação ao tempo
# (para gerar a legenda depois)
def geraVelocidadeMUV(valores, v0, acc, ini, fim):
    tempo = valores[0]
    x = []

    for i in range(len(tempo)):
        # Se for antes do inicio, ainda nao comecou a andar
        if(i < ini):
            x.append(0)
        # Comecou a andar
        elif(i <= fim):
            t = tempo[i] - tempo[ini]
            x.append(v0 + acc * t)
        # Parou
        else:
            x.append(0)
            
    return plt.plot(tempo, x, 'm-', linewidth = 3, alpha = 0.5, label = 'Velocidade')

# Gera o gráfico do arquivo 'nome' do acelerômetro usando a função leArquivo
def geraGrafico(nome, valores):
    tempo = valores[0]
    x = valores[1]
    y = valores[2]
    z = valores[3]
    F = valores[4]

    # Plotamos os títulos
    plt.title(nome)
    plt.xlabel("tempo")
    plt.ylabel("x, y, z, F")

    # Plotamos os gráficos e damos nomes (label) para criarmos as legendas
    plt.plot(tempo, x, 'r-')
    plt.plot(tempo, y, 'b-')
    plt.plot(tempo, z, 'y-')
    plt.plot(tempo, F, 'k-')

    return

# Essa função gera o gráfico completo de MRU de acordo com o nome da chamada
def geraMRU(nome, vMedia, leituras):
    # Gera as médias dos tempos do dicionário
    t1 = (leituras[nome][0][0] + leituras[nome][0][1]) / 2
    t2 = (leituras[nome][1][0] + leituras[nome][1][1]) / 2
    t3 = (leituras[nome][2][0] + leituras[nome][2][1]) / 2

    # Cria a lista de valores (tempo, x, y, z, F) e os índices de início e fim
    # do movimento
    MRU = leArquivo(nome + '.csv')
    ini, fim = IniFim(MRU)

    # Chama as funcoes para plotarem os gráficos
    geraGrafico(nome + '.csv', MRU)
    posicao, = geraPosicaoMRU(MRU, vMedia, ini, fim)

    # Plota as posições de acordo com o cronômetro
    m10, = plt.plot(t1 + MRU[0][ini], 10, 'ro', label = '10m')
    m20, = plt.plot(t2 + MRU[0][ini], 20, 'r^', label = '20m')
    m30, = plt.plot(t3 + MRU[0][ini], 30, 'r*', label = '30m')

    plt.legend(handles = [m10, m20, m30, posicao])


    return

# Recebe as leituras do cronômetro para um experimento de MUV
# e calcula a média das acelerações e da velocidade inicial
# baseado nas medidas 1 e 2, 1 e 3, 2 e 3
def calculaAccMedia(leituras):
    t1 = (leituras[0][0] + leituras[0][1]) / 2
    t2 = (leituras[1][0] + leituras[1][1]) / 2
    t3 = (leituras[2][0] + leituras[2][1]) / 2
    
    a12, v012 = calculaAcc(t1, 10.0, t2, 20.0)    
    a13, v013 = calculaAcc(t1, 10.0, t3, 30.0)    
    a23, v023 = calculaAcc(t2, 20.0, t3, 30.0)
    
    return (a12 + a13 + a23)/3, (v012 + v013 + v023)/3

# Recebe dois tempos e duas distâncias, e calcula a velocidade inicial
# e a aceleração igualando a fórmula x = v0t + a*(t^2)/2
def calculaAcc(t1, d1, t2, d2):
    a = (2*(d1/t1 - d2/t2))/(t1-t2)
    v0 = (d1/t1 - a*t1/2)
    
    return a, v0

# Essa função é basicamente a junção das duas funções acima, que recebe o nome
# da pessoa que terá a velocidade e a aceleração médias calculadas e os tempos
# lidos no cronômetro
def accMedia(nome, leituras):
    acc = 0
    v0 = 0
    accAux = 0
    v0Aux = 0

    # Faz a busca dos tempos da pessoa chamada no argumento da função
    regex = re.escape(nome + r"MUV")
    for key in leituras:
        if(re.search(regex, key)):
            
            # Calcula a aceleração e a velocidade médias
            accAux, v0Aux = calculaAccMedia(leituras[key])
            acc += accAux
            v0 +=v0Aux
    return acc/3, v0/3

# Função que executa todos os comandos para plotar os gráficos e gerar as
# velocidades do experimento de movimento acelerado
def geraMUV(nome, v0Media, accMedia, leituras, modo):
    # Gera as médias dos tempos do dicionário
    t1 = (leituras[nome][0][0] + leituras[nome][0][1]) / 2
    t2 = (leituras[nome][1][0] + leituras[nome][1][1]) / 2
    t3 = (leituras[nome][2][0] + leituras[nome][2][1]) / 2

    # Cria a lista de valores (tempo, x, y, z, F) e os índices de início e fim
    # do movimento
    MUV = leArquivo(nome + '.csv')
    ini, fim = IniFim(MUV)

    # Chama as funções para plotarem os gráficos
    geraGrafico(nome + '.csv', MUV)
    
    
    # Caso a função tenha sido chamada com modo 'posicao', ela plotará o gráfico
    # da simulação da posição, caso contrario plotará o grafico da velocidade
    if (modo == 'posicao'):
        posicao, = geraPosicaoMUV(MUV, v0Media, accMedia, ini, fim)
        m10, = plt.plot(t1 + MUV[0][ini], 10, 'ro', label = '10m')
        m20, = plt.plot(t2 + MUV[0][ini], 20, 'r^', label = '20m')
        m30, = plt.plot(t3 + MUV[0][ini], 30, 'r*', label = '30m')
        plt.legend(handles = [m10, m20, m30, posicao])
    else:
        velocidade, = geraVelocidadeMUV(MUV, v0Media, accMedia, ini, fim)
        plt.legend(handles = [velocidade])
    
    return

# As próximas duas funções calculam os erros entre a simulação e o movimento do
# acelerômetro, gerando o valor absoluto das diferenças da posição de acordo com
# o cronômetro e os valores supostos de posição (10m, 20m e 30m)
def calculaErroMRU(nome, v, leituras):
    nome = nome + '.csv'
    t1 = (leituras[0][0] + leituras[0][1])/2
    t2 = (leituras[1][0] + leituras[1][1])/2     
    t3 = (leituras[2][0] + leituras[2][1])/2
    e1 = abs(t1*v - 10)
    e2 = abs(t2*v - 20)
    e3 = abs(t3*v - 30)
    print ("Erros calculados para o arquivo", nome, "em metros:")
    print ("Observação em 10 metros:", e1)
    print ("Observação em 20 metros:", e2)
    print ("Observação em 30 metros:", e3)
    print ("Erro médio:", (e1 + e2 + e3) / 3)
    print ("\n")
    return

def calculaErroMUV(nome, v0, acc, leituras):
    nome = nome + '.csv'
    t1 = (leituras[0][0] + leituras[0][1])/2
    t2 = (leituras[1][0] + leituras[1][1])/2     
    t3 = (leituras[2][0] + leituras[2][1])/2
    e1 = abs(v0*t1 + (t1**2)*acc/2 - 10)
    e2 = abs(v0*t2 + (t2**2)*acc/2 - 20)
    e3 = abs(v0*t3 + (t3**2)*acc/2 - 30)
    print ("Erros calculados para o arquivo", nome, "em metros:")
    print ("Observação em 10 metros:", e1)
    print ("Observação em 20 metros:", e2)
    print ("Observação em 30 metros:", e3)
    print ("Erro médio:", (e1 + e2 + e3) / 3)
    print ("\n")
    return

# Itera nas três pessoas que realizaram os experimentos, chamando as funções 
# para desenhar o gráfico de cada experimento, além de calcular a velocidade
# e aceleração média de todas as pessoas e os erros  
def main():
    leituras = leCronometro()
    
    pessoas = ['Kei', 'Cerco', 'Joao']
    velocidade = []
    aceleracao = []
    velocidade0 = []
    
    vMedia = 0
    aMedia = 0
    v0Media = 0
    
    # Realiza o bloco a seguir para cada pessoa
    for i in range(len(pessoas)):
        
        # Adicionamos as velocidades e aceleração em listas
        velocidade.append(velMedia(pessoas[i], leituras))
        acc, v0 = accMedia(pessoas[i], leituras)
        aceleracao.append(acc)
        velocidade0.append(v0)
        
        # Adicionamos as velocidades e aceleração para calcular a média depois
        vMedia += velocidade[i]
        aMedia += aceleracao[i]
        v0Media += velocidade0[i]
        
        print("Corredor:", pessoas[i])
        print("Velocidade MRU:", velocidade[i])
        print("Aceleração MUV:", aceleracao[i])
        print("Velocidade Inicial MUV:", velocidade0[i])
        print("\n")
        
        # Criamos uma nova janela para os gráficos de MUV
        plt.figure(i*2 + 1)
        
        # Essa janela terá 6 gráficos, então iteraremos 6 vezes
        for j in range(6):
            
            # Criamos um subplot no lugar apropriado
            plt.subplot(321 + j)
            nome = pessoas[i] + 'MUV' + str(int((j + 2)/2))
            
            # Checamos se devemos fazer gráfico de posição ou de velocidade
            if (j%2 != 0):
                modo = 'posicao'
                calculaErroMUV(nome, velocidade0[i], aceleracao[0], leituras[nome])
            else:
                modo = 'velocidade'
            geraMUV(nome, velocidade0[i], aceleracao[i], leituras, modo)
        
        # Exibimos o gráfico
        plt.show()
        
        # Criamos uma nova janela para os gráficos de MRU
        plt.figure(i*2 + 2)
        
        # Essa janela terá 3 gráficos, então iteraremos 3 vezes
        for j in range(3):
            
            # Criamos um subplot no lugar apropriado
            plt.subplot(311 + j)
            nome = pessoas[i] + 'MRU' + str(j+1)
            geraMRU(nome, velocidade[i], leituras)
            calculaErroMRU(nome, velocidade[i], leituras[nome])
        
        # Exibimos o gráfico
        plt.show()
    
    # Calcula a média das velocidades e aceleração de cada pessoa
    vMedia /= 3
    aMedia /= 3
    v0Media /= 3

    # Exibe as médias calculadas acima    
    print ("Velocidade média de todos os experimentos de MRU (m/s): ", vMedia)
    print ("Aceleração média de todos os experimentos de MUV (m/s²): ", aMedia)
    print ("Velocidade inicial média de todos os experimentos de MUV (m/s): ", vMedia)

    return
main()