/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* No Reinado de MACOEZE
*
***************************************************************/

public class BattleForTheKingdom
{
  public static void main(String[] args)
  {
    // Garantido que cada variável estará no respectivo intervalo
    int N = StdIn.readInt();  // 3 <= N <= 10.000    - Número de cidades
    int M = StdIn.readInt();  // 3 <= M <= 1.000.000 - Número de estradas
    int K = StdIn.readInt();  // 1 <= K <= 200       - Número de heróis

    // Cidade inicial de cada um dos K heróis e do feiticeiro F
    int[] heroes = new int[K];
    int F;
    // Quais cidades são seguras
    Queue<Integer> safeCities = new Queue<Integer>();

    // Constrói o reino (digraph)
    EdgeWeightedDigraph EWD = new EdgeWeightedDigraph(N);
    for (int i = 0; i < M; i++)
    {
      int A = StdIn.readInt();
      int B = StdIn.readInt();
      int T = StdIn.readInt();

      DirectedEdge E = new DirectedEdge(A, B, (double) T);
      EWD.addEdge(E);
    }

    // Posiciona cada um dos K heróis em sua cidade inicial
    // e constrói o shortest path de cada um
    DijkstraSP[] heroSP = (DijkstraSP[]) new DijkstraSP[K];
    for (int i = 0; i < K; i++)
    {
      heroes[i] = StdIn.readInt();
      heroSP[i] = new DijkstraSP(EWD, heroes[i]);
    }

    // Posiciona o feiticeiro em sua cidade inicial
    F = StdIn.readInt();
    DijkstraSP sorcSP = new DijkstraSP(EWD, F);

    // Verifica quais cidade são seguras
    for (int n = 0; n < N; n++)
    {
      int longest = 0; // Herói que leva mais tempo para chegar na cidade n
      for (int k = 0; k < K; k++)
        if (heroSP[k].distTo(n) > longest)
          longest = (int) heroSP[k].distTo(n);

      // Se o feiticeiro levar mais tempo que o herói mais demorado para chegar na cidade n, então a cidade n é segura
      if (sorcSP.distTo(n) > longest)
        safeCities.enqueue(n);
    }

    if (!safeCities.isEmpty())
    {
      int size = safeCities.size();
      StdOut.println("VICTORY AND HAPPY EVER AFTER");
      StdOut.println(size);
      for (int s = 0; s < size; s++)
      {
        int city = safeCities.dequeue();
        StdOut.print(city + " ");
      }
      StdOut.println();
    }
    else
      StdOut.println("DEMISE OF THE KINGDOM");
  }
}
