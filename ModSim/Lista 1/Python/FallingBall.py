class FallingBall:
  g = 9.8

  def __init__ (self):
    print "A new FallingBall object was created."
  
  def step(self):
    self.y += self.v * self.dt
    self.v -= self.g * self.dt
    self.t += self.dt
  
  def analyticPosition(self, y0, v0):
    return y0 + v0*self.t  - 0.5*self.g*self.t*self.t
  
  def analyticVelocity(self, v0):
    return v0 - self.g*self.t
