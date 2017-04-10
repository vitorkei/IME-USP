/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vitor Kei Taira Tamada
* NUSP: 8516250
* Problema da Conexidade 2D
*
***************************************************************/

public class LC2D
{

  // Retorna a media
  public static double media (int[] N)
  {
    double media = 0;
    int i;
    for (i = 0; i < N.length; i++)
      media += N[i];
    media /= i;

    return media;
  }

  // Retorna a variância
  public static double variancia (double media, int[] N)
  {
    double var = 0;
    int i;
    for (i = 0; i < N.length; i++)
      var += Math.pow (media - N[i], 2);
    var /= (i - 1);
    return var;
  }
  public static void main (String[] args)
  {
//    long startTime = System.nanoTime();
    // Declaração de variáveis
    // A estrutura utilizada é o UF
    double d = Double.parseDouble (args[0]);
    int T = Integer.parseInt (args[1]);
    int S;
    if (args.length == 3)
    {
      S = Integer.parseInt (args[2]);
      StdRandom.setSeed(S);
    }
    int[] medias = new int[T];
    double media;
    double var;

    DynamicWeightedQuickUnionUF uf = new DynamicWeightedQuickUnionUF();

    int G = (int) (Math.ceil (1.0 / d));
/*
    for (int t = 0; t < T; t++)
    {
*/
      int n;
      ST<Integer, Point2D>[][] grid = (ST<Integer, Point2D>[][]) new ST[G + 2][G + 2];
      for (int i = 0; i < G + 2; i++)
        for (int j = 0; j < G + 2; j++)
          grid[i][j] = new ST<Integer, Point2D>();

      // Plota os pontos
      // É necessário plotar dois pontos e verificar se eles estão conexos, pois 1 ponto sozinho forma uma componente pela UF, o que indica que há apenas uma classe de equivalência
      for (n = 0; n < 2; n++)
      {
        double x = StdRandom.uniform();
        double y = StdRandom.uniform();
        Point2D p = new Point2D (x, y);

        StdDraw.text (x, y, Integer.toString(n));

        int row = (int) (Math.ceil (x * G));
        int col = (int) (Math.ceil (y * G));
        grid[row][col].put (n, p);

        for (int i = row - 1; i < row + 2; i++)
          for (int j = col - 1; j < col + 2; j++)
            if (i >= 0 && i < G + 2 && j >= 0 && j < G + 2)
              for (int m : grid[i][j].keys())
              {
                Point2D q = grid[i][j].get (m);
                StdOut.println ("n = " + n + "\nm = " + m + "\nd = " + p.distanceTo(q) + "\n=========");
                if (p.distanceTo (q) <= d)
                {
                  uf.union (n, m);
                  StdDraw.line (p.x(), p.y(), q.x(), q.y());
                }
              }
  //      StdOut.println (p.x() + " " + p.y());
        StdOut.println ("count = " + uf.count() + "\n**************");
      }
      for (n = 2; uf.count() != 1; n++)
      {
        double x = StdRandom.uniform();
        double y = StdRandom.uniform();
        Point2D p = new Point2D (x, y);

        StdDraw.text (x, y, Integer.toString(n));

        int row = (int) (Math.ceil (x * G));
        int col = (int) (Math.ceil (y * G));
        grid[row][col].put (n, p);

        for (int i = row - 1; i < row + 2; i++)
          for (int j = col - 1; j < col + 2; j++)
            if (i >= 0 && i < G + 2 && j >= 0 && j < G + 2)
              for (int m : grid[i][j].keys())
              {
                Point2D q = grid[i][j].get(m);
                StdOut.println ("n = " + n + "\nm = " + m + "\nd = " + p.distanceTo(q) + "\n=========");
                if (p.distanceTo (q) <= d)
                {
                  uf.union (n, m);
                  StdDraw.line(p.x(), p.y(), q.x(), q.y());
                }
              }
        StdOut.println ("count = " + uf.count() + "\n**************");
      }
/*
      medias[t] = n;
    }

    media = media (medias);
    var = variancia (media, medias);

    StdOut.println (media + " " + var);
*/
    StdOut.println ("Sim");
/*
    long stopTime = System.nanoTime ();
    double deltaTime = (double) (stopTime - startTime) / 1000000000;
    StdOut.println ("delta = " + deltaTime);
*/
  }
}
