public class FallingBallApp
{
  public static void main (String[] args)
  {
    FallingBall ball = new FallingBall();
    FallingBall ball2 = new FallingBall();
    
    double y0 = 10;
    double v0 = 0;
    
    ball.t = 0;
    ball.dt = 0.001;
    ball.y = y0;
    ball.v = v0;
    
    ball2.t = 0;
    ball2.dt = 0.0001;
    ball2.y = y0;
    ball2.v = v0;
    
    while (ball.y > 0)
    {
      ball.step();
      ball2.step();
    }
    
    System.out.println("Results");
    System.out.println("final time = " + ball.t);
    
    System.out.println("\nBALL 1");
    System.out.println("y = " + ball.y + " || v = " + ball.v);
    
    System.out.println("analytic y = " + ball.analyticPosition(y0, v0));
    System.out.println("analytic v = " + ball.analyticVelocity(v0));
    
    System.out.println("\nBALL 2");
    System.out.println("y = " + ball2.y + " || v = " + ball2.v);
    System.out.println("analytic y = " + ball2.analyticPosition(y0, v0));
    System.out.println("analytic v = " + ball2.analyticVelocity(v0));
    
    System.out.println("\nacceleration = " + FallingBall.g);
  }
}
