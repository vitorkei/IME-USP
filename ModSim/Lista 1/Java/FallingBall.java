public class FallingBall
{
  double y, v, t;
  double dt;
  final static double g = 9.8;
  
  public FallingBall()
  {
    System.out.println("A new FallingBall object is created.");
  } 
  public void step()
  {
    y = y + v * dt;
    v = v - g * dt;
    t = t + dt;
  }
  
  public double analyticPosition (double y0, double v0)
  {
    return y0 + v0 * t - 0.5 * g * t * t;
  }
  
  public double analyticVelocity (double v0)
  {
    return v0 - g * t;
  }
}
