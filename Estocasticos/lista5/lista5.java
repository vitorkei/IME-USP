import java.util.Random;

public class lista5
{
  static Random rand = new Random();

  public static double coinFlip(double p)
  {
    if(rand.nextDouble() < p) return 1.0;

    return -1.0;
  }
  public static void main (String args[])
  {
    // Desenha os eixos do gráfico
    StdDraw.setXscale(-10, 510);
    StdDraw.setYscale(-5, 5);
    StdDraw.line(0, -3, 0, 3); // Ordenada
    StdDraw.line(0, 0, 505, 0); // Abscissa
    StdDraw.show();

    double S = 0;
    double p = 0.5, x0 = 0, y0 = 0;
    Random rand = new Random();
    for(int k = 0; k < 3; k++)
    {
      if(k == 0) StdDraw.setPenColor(StdDraw.BLACK);
      else if(k == 1) StdDraw.setPenColor(StdDraw.RED);
      else if(k == 2) StdDraw.setPenColor(StdDraw.BLUE);
      S = 0; x0 = 0; y0 = 0;
      for(int i = 0; i < 500; i++)
      {
        S += coinFlip(p);
        StdDraw.line(x0, y0, x0+1, S/((double) i+1));
        x0++;
        y0 = S/((double) i+1);
      }
    }
  }
}
