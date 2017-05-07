from Particle import Particle

class FallingParticle(Particle):
  g = 9.8
  __y0 = 0
  __v0 = 0
  
  def __init__ (self, y, v):
    print "A new FallingParticle object was created."
    self.y = y
    self.v = v
    self.__y0 = y
    self.__v0 = v
  
  def step(self):
    #print "fallingParticle method"
    self.v -= self.g * self.dt
    self.y += self.v * self.dt
    self.t += self.dt
  
  def analyticPosition(self):
    return self.__y0 + self.__v0 * self.t - (self.g * self.t * self.t) / 2.0
  
  def analyticVelocity(self):
    return self.__v0 - self.g * self.t
