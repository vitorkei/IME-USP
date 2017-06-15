import matplotlib.pyplot as plt
import math

x00 = 0
step = 0.01
nsteps = int(31.42/step)

def yEuler(y0, dx, x):
    return y0 + dx*math.cos(x)

def rodaEuler():
    x0 = x00
    y0 = 0
    x = []
    yE = []
    yA = []
    erro = []
    for i in range(nsteps):
        x.append(i*step)
        y0 = yEuler(y0, step, i*step)
        yE.append(y0)
        yA.append(math.sin(i*step))
        erro.append(abs(yE[i] - yA[i]))

    plt.plot(x, yE, '-', x, yA, '-', x, erro, '-')
    plt.show()

rodaEuler()
