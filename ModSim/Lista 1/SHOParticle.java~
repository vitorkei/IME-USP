public class SHOParticle extends Particle
{
  private double y0 = 0, v0 = 0;
  private double k = 1.0;
  private double omega0 = Math.sqrt(k);
  
  public SHOParticle(double y, double v)
  {
    System.out.println("A new SHOParticle object was created.");
    this.y = y;
    this.v = v;
    y0 = y;
    v0 = v;
  }
  
  public void step()
  {
    v = v - k * y * dt;
    y = y + v * dt;
    t = t + dt;
  }
  
  public double analyticPosition()
  {
    return y0 * Math.cos(omega0 * t) + v0 / omega0 * Math.sin(omega0 * t);
  }
  
  public double analyticVelocity()
  {
    return -y0 * omega0 * Math.sin(omega0 * t) + v0 * Math.cos(omega0 * t);
  }
}
