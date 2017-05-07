public class FallingParticleApp
{
  public static void main(String[] args)
  {
    Particle ball = new FallingParticle(10, 0);
    ball.t = 0;
    ball.dt = 0.01;
    while (ball.y > 0)
      ball.step();
    
    System.out.println("Results");
    System.out.println("final time = " + ball.t);
    System.out.println("\ny = " + ball.y + "\nv = " + ball.v);
    System.out.println("}\nyAnalytic = " + ball.analyticPosition());
  }
}
