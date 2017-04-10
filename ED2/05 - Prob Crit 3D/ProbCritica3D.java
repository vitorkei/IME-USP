/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Probabilidade Crítica 3D
*
***************************************************************/

public class ProbCritica3D
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

  public static double probCrit (int N, int M, int max, double lim, int a, int b)
  {
    double p, aux;
    double[] medias = new double [1000]; // p começa em 0 e aumenta em .001 por iteração do laço, totalizando 1000 iterações. O ponto crítico, então, será um número com 4 digitos significativos

    // Gera a matriz e tenta percolá-la
    // O gráfico é plotado conforme as matrizes são testadas
    for (p = 0; p < 1.0; p = p + .001)
    {
      aux = 0.0;
      for (int i = 0; i < M; i++)
      {
        boolean[][][] open = random (N, p);
        if (percola (open))
          aux++;
      }
      aux = aux / M;
      double p_aux = p * 1000;
      medias[(int) p_aux] = aux - .5; // Desta forma, o ponto em que a probabilidade de percolar é de 50% ficará no ponto (p_c, 0), sendo p_c o p crítico que está sendo buscado
    }

    // Método da bisecção
    for (int j = 0; j < max; j++)
    {
      int c = (a + b) / 2;
      if (medias[c] == 0 || (b - a) / 2 < lim)
        return c;

      if (medias[c] * medias[a] >= 0)
        a = c;
      else
        b = c;
    }
    StdOut.println ("Número máximo de iterações excedido");
    return -1;
  }

  public static void main (String[] args)
  {
    int N = StdIn.readInt ();
    int M = StdIn.readInt ();

    StdOut.println (probCrit (N, M, 1000, .0001, 0, 999) / 1000);
  }
}
