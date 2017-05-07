from Particle import Particle
import math

class SHOParticle(Particle):
  y0 = 0
  v0 = 0
  k = 1.0
  omega0 = math.sqrt(k)
  
  def __init__(self, y, v):
    print "A new harmonic oscillator object was created."
    self.y = y
    self.v = v
    self.y0 = y
    self.v0 = v
  
  def step(self):
    print "SHOParticle"
    self.v -= self.k * self.y * self.dt
    self.y += self.v * self.dt
    self.t += self.dt
  
  def analyticPosition(self, y0, v0):
    return self.y0 * math.cos(self.omega0 * self.t) + v0 / self.omega0 * math.sin(self.omega0 * self.t)
  
  def analyticVelocity(self, y0, v0):
    return -self.y0 * self.omega0 * math.sin(self.omega0 * self.t) + v0 * math.cos(self.omega0 * self.t)
