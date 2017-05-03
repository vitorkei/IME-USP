public class FallingBallApp
{
  public static void main (String[] args)
  {
    FallingBall ball = new FallingBall();
    double y0 = 10;
    double v0 = 0;
    ball.t = 0;
    ball.dt = 0.001;
    ball.y = y0;
    ball.v = v0;
    
    while (ball.y > 0)
      ball.step();
    
    System.out.println("Results");
    System.out.println("final time = " + ball.t);
    
    System.out.println("y = " + ball.y + " || v = " + ball.v);
    
    System.out.println("analytic y = " + ball.analyticPosition(y0, v0));
    System.out.println("analytic v = " + ball.analyticVelocity(v0));
    System.out.println("acceleration = " + FallingBall.g);
  }
}
