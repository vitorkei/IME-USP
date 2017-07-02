public class fundamentalDiagram
{
  public static void main(String[] args)
  {
    double flowRate, density;
    
    // Escala do diagrama
    StdDraw.setCanvasSize();
    StdDraw.setXscale(-0.075, 0.51);
    StdDraw.setYscale(-0.2, 1.1);
    
    // Eixos horizontal e vertical
    StdDraw.line(0, 0, 0.5, 0);
    StdDraw.line(0, 0, 0, 1);
    
    // Marcações do eixo x
    StdDraw.line(0.125, -0.01, 0.125, 0.01);
    StdDraw.line(0.25, -0.01, 0.25, 0.01);
    StdDraw.line(0.375, -0.01, 0.375, 0.01);
    StdDraw.line(0.5, -0.01, 0.5, 0.01);
    
    // Marcações do eixo y
    StdDraw.line(-0.005, 0.25, 0.005, 0.25);
    StdDraw.line(-0.005, 0.5, 0.005, 0.5);
    StdDraw.line(-0.005, 0.75, 0.005, 0.75);
    StdDraw.line(-0.005, 1, 0.005, 1);
    
    // Legendas do diagrama
    StdDraw.text(-0.05, 0.5, "Density", 90);
    StdDraw.text(0.25, -0.1, "Flow rate");
    
    // Legendas das marcações do eixo x
    StdDraw.text(0, -0.04, "0");
    StdDraw.text(0.125, -0.04, "0.125");
    StdDraw.text(0.25, -0.04, "0.25");
    StdDraw.text(0.375, -0.04, "0.035");
    StdDraw.text(0.495, -0.04, "0.5");
    
    // Legendas das marcações do eixo y
    StdDraw.text(-0.025, 0, "0");
    StdDraw.text(-0.03, 0.25, "0.25");
    StdDraw.text(-0.025, 0.5, "0.5");
    StdDraw.text(-0.03, 0.75, "0.75");
    StdDraw.text(-0.02, 1, "1");
    
    StdDraw.setPenColor(StdDraw.RED);
    while(!StdIn.isEmpty())
    {
      flowRate = StdIn.readDouble();
      density = StdIn.readDouble();
      StdDraw.point(flowRate, density);
    }
    System.out.println("fim");
  }
}
