import java.lang.Math;

public class FirstFallingBallApp
{
  public static void main (String[] args)
  {
    double y0 = 10;
    double v0 = 0;
    double t = 0;
    double dt = 0.001;
    double y = y0;
    double v = v0;
    double g = 9.8;
    
    while(y > 0)
    {
      y = y + v * dt;
      v = v - g * dt;
      t = t + dt;
    }
    
    System.out.println ("Results");
    System.out.println ("final time = " + t);
    System.out.println ("y = " + y + " || v = " + v);
    double yAnalytic = y0 + v0 * t - 0.5 * g * t * t;
    double vAnalytic = v0 - g * t;
    System.out.println ("analytic y = " + yAnalytic + " || v = " + vAnalytic);
    
    System.out.println ("Error");
    System.out.println ("y: " + Math.abs((y - yAnalytic) / (y + yAnalytic)) + " || v: " + Math.abs((v - vAnalytic) / (v + vAnalytic)));
  }
}
