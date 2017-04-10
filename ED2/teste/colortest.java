public class colortest
{
  public static void main (String[] args)
  {
    int L = Integer.parseInt (args[0]);
//    StdDraw.setScale (0, 255);

    StdDraw.setScale (0, 2.2*L);  // As escalas estão de tal forma que tenha o quadrado de lado 2L e uma margem em volta
    StdDraw.setPenColor (0, 0, 255);
    StdDraw.filledSquare (L, L, L);
/*
    for (int i = 0; i < 256; i++)
    {
      StdDraw.setPenColor (i, 0, 255 - i);
      StdDraw.filledRectangle ((double) i, 125.5, .5, 100);
    }
*/
  }
}
