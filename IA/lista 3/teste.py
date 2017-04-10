if __name__ == '__main__':
  d = {}
  e = {'(3, 3)' : 3, '(4, 4)' : 100}
  print "e"
  print e
  print "\nfor i in e"
  for i in e:
    print "i = ", i
  print "\nfor i in e.items()"
  for i in e.items():
    print i
  print"\nfor i in e: print e[i]"
  for i in e:
    print e[i]
  print "\nd ", d
  d['(1, 1)'] = 0
  e = d.copy()
  print "d1 ", d
  print "e ", e
  e['(2, 2)'] = 1
  e['(1, 1)'] = 2
  print "d2 ", d
  print "e1 ", e
  l = []
  print l
  l.append(d)
  print l
  print l[0]
