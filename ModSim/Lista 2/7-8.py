import random
import sys

# Imprime a grid
def printGrid(g, n):
  for i in range (n):
    for j in range(n):
      sys.stdout.write(str(g[i][j]))
      sys.stdout.write(" ")
    sys.stdout.write("\n")
  sys.stdout.write("\n")
  sys.stdout.flush()
  print(chr(27) + "[2J")

def randWalk2D():
  n = 30
  M = 10
  w = [[0 for x in range(2)] for y in range(M)]
  dx =[0 for x in range(4)]
  
  # Inicializa a grid
  grid = [[0 for x in range(n)] for y in range(n)]
  for i in range(n):
    for j in range(n):
      grid[i][j] = "."
  
  # Posiciona os andarilhos aleatoriamente na grid
  for m in range(M):
    x0, y0 = 0, 0
    while not grid[x0][y0] == ".":
      x0 = random.randint(0, n-1)
      y0 = random.randint(0, n-1)
    grid[x0][y0] = "W"
    w[m][0] = x0
    w[m][1] = y0
  
  printGrid(grid, n)
  
  for i in range(1000):
    for m in range(M):
      x = w[m][0] # linha do andarilho atual
      y = w[m][1] # coluna do andarilho atual
      flag = True
      d = -1
      #print "m = ", m
      for i in range(4):
        dx[i] = 0
      while flag:
        count = 0
        for i in range(4):
          if dx[i] == 1:
            count +=1
        if count == 4:
          break
        d = random.randint(0, 3)
        if d == 0: # anda para cima
          if x-1 >= 0 and grid[x-1][y] == ".":
            grid[x][y] = "."
            grid[x-1][y] = "W"
            w[m][0] = x-1
            w[m][1] = y
            #print "d = cima"
            flag = False
          else:
            dx[d] = 1
        elif d == 1: # anda para a direita
          if y+1 < n and grid[x][y+1] == ".":
            grid[x][y] = "."
            grid[x][y+1] = "W"
            w[m][0] = x
            w[m][1] = y+1
            #print "d = direita"
            flag = False
          else:
            dx[d] = 1
        elif d == 2: # anda para baixo
          if x+1 < n and grid[x+1][y] == ".":
            grid[x][y] = "."
            grid[x+1][y] = "W"
            w[m][0] = x+1
            w[m][1] = y
            #print "d = baixo"
            flag = False
          else:
            dx[d] = 1
        elif d == 3: # anda para a esquerda
          if y-1 >= 0 and grid[x][y-1] == ".":
            grid[x][y] = "."
            grid[x][y-1] = "W"
            w[m][0] = x
            w[m][1] = y-1
            #print "d = esquerda"
            flag = False
          else:
            dx[d] = 1
    printGrid(grid, n)


randWalk2D()
