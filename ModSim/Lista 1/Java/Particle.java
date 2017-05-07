abstract public class Particle
{
  double y, v, t;
  double dt;
  
  public Particle()
  {
    System.out.println("a new particle was created");
  }
  
  abstract protected void step();
  abstract protected double analyticPosition();
  abstract protected double analyticVelocity();
}
