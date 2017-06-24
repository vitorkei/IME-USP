def calculate():
  r = 0.26
  x0 = 0.5
  print 'r = ', r, ' || x0 = ', x0
  x = x0
  for i in range (10000):
    x = 4 * r * x0 * (1-x0)
    x0 = x
  
  print "x = ", x

calculate()
