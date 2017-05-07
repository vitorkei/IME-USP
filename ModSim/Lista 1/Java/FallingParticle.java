public class FallingParticle extends Particle
{
  final static double g = 9.8;
  private double y0 = 0, v0 = 0;
  
  public FallingParticle(double y, double v)
  {
    System.out.println("A new FallingParticle object was created.");
    this.y = y;
    this.v = v;
    y0 = y;
    v0 = v;
  }
  
  public void step()
  {
    //System.out.println("blub");
    v = v - g * dt;
    y = y + v * dt;
    t = t + dt;
  }
  
  public double analyticPosition()
  {
    return y0 + v0 * t - (g * t * t) / 2.0;
  }
  
  public double analyticVelocity()
  {
    return v0 - g * t;
  }
}