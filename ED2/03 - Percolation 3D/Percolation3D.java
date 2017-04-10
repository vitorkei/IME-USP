/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Creative Exercise 2.4.18 (Percolation in three dimensions; IntroCS)
*
***************************************************************/

public class Percolation3D
{
  // Dada uma matriz NxNxN com alguns campos abertos, retorna uma matriz NxNxN idêntica percolada
  public static boolean[][][] flow (boolean[][][] open)
  {
    int N = open.length;
    boolean[][][] full = new boolean [N][N][N];

    for (int k = 0; k < N; k++)
      for (int j = 0; j < N; j++)
        flow (open, full, 0, j, k);

    return full;
  }

  // Determina o conjunto de campos "cheios" utilizando DFS
  public static void flow (boolean[][][] open, boolean[][][] full, int i, int j, int k)
  {
    int N = open.length;

    // Casos base
    if (i < 0 || i >= N) return;
    if (j < 0 || j >= N) return;
    if (k < 0 || k >= N) return;
    if (!open[i][j][k]) return;
    if (full[i][j][k]) return;

    // Marca (i, j, k) como cheio
    full[i][j][k] = true;

    flow (open, full, i + 1, j, k); // baixo
    flow (open, full, i, j + 1, k); // direita
    flow (open, full, i, j - 1, k); // esquerda
    flow (open, full, i - 1, j, k); // cima
    flow (open, full, i, j, k + 1); // trás (?)
    flow (open, full, i, j, k - 1); // frente (?)
  }

  // Verifica se o sistema percola
  public static boolean percola (boolean[][][] open)
  {
    int N = open.length;
    boolean[][][] full = flow (open);

    for (int k = 0; k < N; k++)
      for (int j = 0; j < N; j++)
        if (full[N - 1][j][k])
          return true;

    return false;
  }

  // Gera uma matriz booleana NxNxN aleatória
  public static boolean[][][] random (int N, double p)
  {
    boolean[][][] a = new boolean [N][N][N];

    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        for (int k = 0; k < N; k++)
          a[i][j][k] = StdRandom.bernoulli (p);

    return a;
  }

  public static void main (String[] args)
  {
    int N = StdIn.readInt ();
    int M = StdIn.readInt ();
    int i;
    double p, media, aux, inc;  // inc = increase (rate)

    // Desenha o gráfico sem os pontos plotados
    StdDraw.setScale (-.1, 1.1);
    StdDraw.line (0, 0, 1, 0);
    StdDraw.line (0, 0, 0, 1);
    StdDraw.text (1, -.025, "p");
    StdDraw.text (.1, 1.02, "Prob. de percolar");
    StdDraw.text (-.025, -.025, "0");

    // Gera a matriz e tenta percolá-la
    // O gráfico é plotado conforme as matrizes são testadas
    aux = 0.0;
    inc = .01;
    for (p = inc; p < 1.0; p = p + inc)
    {
      media = 0.0;
      for (i = 0; i < M; i++)
      {
        boolean[][][] open = random (N, p);
        if (percola (open))
          media++;
      }
      media = media / M;
      StdDraw.filledCircle (p, media, .0035);
      StdDraw.line (p - inc, aux, p, media);    // Vide READ ME
      aux = media;
    }
  }
}
