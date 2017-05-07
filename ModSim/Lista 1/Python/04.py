if __name__ == '__main__':
  y0 = 10
  v0 = 0
  t = 0
  dt = 0.001
  y = y0
  v = v0
  g = 9.8
  
  for i in range(1430):
    y = y + v * dt
    v = v - g * dt
    t = t + dt
    
  yAnalytic = y0 + v0 * t - 0.5 * g * t * t
  vAnalytic = v0 - g * t
  
  print "Results"
  print "final time = ", t
  
  print "\nEULER:\ny = ", y, "\nv = ", v
  
  
  print "\nANALYTIC:\nanalytic y = ", yAnalytic, "\nv = ", vAnalytic, "\n"
  
  print "ERROR\nAbsoluto(y): ", abs(y - yAnalytic)
  if y > yAnalytic:
    print "Relativo1(y): ", abs(yAnalytic / y)
  else:
    print "Relativo2(y): ", abs(y / yAnalytic)
  
  print "Absoluto(v): ", abs(v - vAnalytic)
  if v > vAnalytic:
    print "Relativo1(v): ", abs(vAnalytic / v)
  else:
    print "Relativo2(v): ", abs(v / vAnalytic)
