/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vitor Kei Taira Tamada
* NUSP: 8516250
* Problema da Conexidade 2D
*
***************************************************************/

public class PC2D
{
  public static void main (String[] args)
  {
//    long startTime = System.nanoTime();
    // Declaração de variáveis
    // A estrutura utilizada é o UF
    int N = StdIn.readInt();
    double d = StdIn.readDouble();
    UF uf = new UF (N);

    int G = (int) (Math.ceil (1.0 / d));

    ST<Integer, Point2D>[][] grid = (ST<Integer, Point2D>[][]) new ST[G + 2][G + 2];
    for (int i = 0; i < G + 2; i++)
      for (int j = 0; j < G + 2; j++)
        grid[i][j] = new ST<Integer, Point2D>();

    // Plota os pontos
    // Verifica quais pontos próximos distam d ou menos do ponto plotado
    for (int n = 0; n < N; n++)
    {
      double x = StdIn.readDouble();
      double y = StdIn.readDouble();
      Point2D p = new Point2D (x, y);

      int row = (int) (Math.ceil (x * G));
      int col = (int) (Math.ceil (y * G));
      grid[row][col].put (n, p);

      for (int i = row - 1; i < row + 2; i++)
        for (int j = col - 1; j < col + 2; j++)
          if (i >= 0 && i < G + 2 && j >= 0 && j < G + 2)
            for (int m : grid[i][j].keys())
              if (p.distanceTo (grid[i][j].get(m)) <= d)
                uf.union (n, m);
    }

    // Verifica se há apenas uma classe de equivalência
    if (uf.count() == 1) StdOut.println ("Sim");
    else                 StdOut.println ("Nao");

/*
    long stopTime = System.nanoTime ();
    double deltaTime = (double) (stopTime - startTime) / 1000000000;
    StdOut.println ("delta = " + deltaTime);
*/
  }
}
