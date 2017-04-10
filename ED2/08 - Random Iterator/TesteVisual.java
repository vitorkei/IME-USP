/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Creative Problem 1.3.36 (Random iterator; Algs4)
*
***************************************************************/

public class TesteVisual
{
  // Retorna o fatorial de N
  public static int factorial (int N)
  {
    int res = 1;
    for (int i = 2; i <= N; i++)  res = res * i;
    return res;
  }

  // Verifica se dois vetores são iguais
  public static boolean areEqual (int[] a, int[] b)
  {
    if (a != null && b != null)
    {
      if (a.length != b.length)
        return false;

      for (int i = 0; i < a.length; i++)
        if (a[i] != b[i])
          return false;

      return true;
    }
    return false;
  }

  // Gera uma permutação || genPerm == generate permutation
  public static int[] genPerm (int N, RandomIterator<Integer> ri)
  {
    int j = 0;
    int[] perm = new int [N];

    for (int num : ri)
    {
      perm[j] = num;
      j++;
    }

    return perm;
  }

  public static void main (String[] args)
  {
    int N = Integer.parseInt (args[0]);
    int T = Integer.parseInt (args[1]);

    // Se T (número de permutações feitas) for maior do que o número máximo de permutações possíveis, faz um vetor que recebe, no máximo, N! Caso contrário, faz um vetor que recebe T permutações
    int T_aux;
    if (T > factorial (N))  T_aux = factorial (N);
    else                    T_aux = T;

    int[][] perm = new int [T][N]; // Serão feitas T iterações, cada uma com N elementos || perm == permutação
    int[] aux = new int [N];       // Array auxiliar para verificar se a permutação já apareceu
    int[] count = new int [T];     // Array que conta quantas vezes cada permutação aparece

    RandomIterator<Integer> ri = new RandomIterator<Integer> ();

    // Insere os 1~N na fila
    for (int i = 0; i < N; i++)
      ri.enqueue (i);

    // Gera T permutações
    for (int i = 0; i < T_aux; i++)
    {
      // Gera uma permutação
      aux = genPerm (N, ri);

      // Verifica se a permutação acima já apareceu antes
      // Em caso positivo, aumenta a respectiva frequência em 1
      int j;                  // Variável auxiliar
      boolean flag = false;   // Flag auxiliar
      for (j = 0; j < i && !flag; j++)    // Busca apenas pelas permutações que já foram feitas, não verificando os espaços ainda "vazios" do vetor
        if (areEqual (aux, perm[j]))
        {
          i--;          // Vide README
          T_aux--;      // Vide README
          flag = true;
        }
      // Em caso negativo, salva ela e marca que apareceu 1 vez
      if (!flag)
        for (int k = 0; k < aux.length; k++)
          perm[j][k] = aux[k];
      count[j]++;
    }

    // Desenha um histograma cuja imagem terá as mesmas dimensões
    // do teste feito no main do Histogram.java.
    // Vide README
    Histogram hist = new Histogram (T_aux);
    for (int i = 0; i < T_aux; i++)
      for (int j = 0; j < count[i]; j++)
        hist.addDataPoint (i);
    StdDraw.setCanvasSize (1000, 500);
    hist.draw ();

    // Caso queira saber quantas vezes houve repetição de permutação
//    StdOut.println ("Há " + (T - T_aux) + " permutações repetidas");
  }
}
