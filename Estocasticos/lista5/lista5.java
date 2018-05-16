// Nome: Vítor Kei Taira Tamada
// NUSP: 8516250
// StdDraw é uma biblioteca do IntroCS
// API: https://introcs.cs.princeton.edu/java/15inout/javadoc/StdDraw.html

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
    int arg = Integer.parseInt(args[0]);
    // Desenha os eixos do gráfico
    
    StdDraw.setXscale(-50, 550);
    StdDraw.setYscale(-55, 55);
    StdDraw.line(0, 0, 500, 0); // Abscissa
    StdDraw.line(0, -50, 0, 50); // Ordenada

    // Linhas e textos de apoio ao gráfico
    // Para Xn
    StdDraw.text(-20, 50, "50");
    StdDraw.text(-22.5, -50, "-50");
    StdDraw.text(525, 0, "500");
    StdDraw.line(-5, 50, 5, 50);
    StdDraw.line(-5, -50, 5, -50);
    StdDraw.line(500, 1, 500, -1);

    // Para Yn e Zn
    StdDraw.text(-17.5, 2, "1");
    StdDraw.text(-20, -2, "-1");
    StdDraw.line(-5, 1, 5, 1);
    StdDraw.line(-5, -1, 5, -1);
    StdDraw.show();

    double S = 0;
    double p = 0.5, x0 = 0, y0 = 0;
    Random rand = new Random();
    for(int k = 0; k < 3; k++)
    {
      if     (k == 0) StdDraw.setPenColor(StdDraw.BLACK);
      else if(k == 1) StdDraw.setPenColor(StdDraw.RED);
      else if(k == 2) StdDraw.setPenColor(StdDraw.BLUE);

      S = 0; x0 = 0; y0 = 0;
      
      if(arg == 0)
      {
        for(int i = 0; i < 500; i++)
        {
          // Para Xn
          S += coinFlip(p);
          StdDraw.line(x0, y0, x0+1, S);
          x0++;
          y0 = S;
        }
      }

      // Para Yn
      else if(arg == 1)
      {
        // Tecnicamente, são infinitas iterações,
        // mas precisamos de um limite aqui
        for(int i = 0; i < 500; i++)
        {
          S += coinFlip(p);
          StdDraw.line(x0, y0, x0+1, S/((double) i+1));
          x0++;
          y0 = S/((double) i+1);
        }
      }

      // Para Zn
      else if(arg == 2)
      {
        double Xi, Xj, Y;
        Xi = coinFlip(p);
        for(int i = 1; i < 500; i++)
        {
          Xj = coinFlip(p);
          Y = (Xi + Xj)/2;
          StdDraw.line(x0, y0, x0+1, Y);
          x0++;
          y0 = Y;
          Xi = Xj;
        }
      }
    }
  }
}
