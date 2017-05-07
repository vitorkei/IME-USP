from FallingParticle import FallingParticle

if __name__ == '__main__':
  ball = FallingParticle(10, 0)
  ball.t = 0
  ball.dt = 0.01
  
  while ball.y > 0:
    ball.step()
  
  print "Results"
  print "final time = ", ball.t
  
  print "\nBALL"
  print "y = ", ball.y, "\nv = ", ball.v
  print "\nyAnalytic = ", ball.analyticPosition()
  print "vAnalytic = ", ball.analyticVelocity()
