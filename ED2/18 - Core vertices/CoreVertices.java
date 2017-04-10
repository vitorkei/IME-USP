/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Web Exercise 4.2.35 (Core vertices; Algs4)
*
***************************************************************/

public class CoreVertices
{
  public static void main(String[] args)
  {
    int n = StdIn.readInt();
    int m = StdIn.readInt();
    Digraph G = new Digraph(n);

    // Constrói o digraph
    for (int i = 0; i < m; i++)
    {
      int a = StdIn.readInt();
      int b = StdIn.readInt();

      G.addEdge(a, b);
    }

    // Cria as componentes fortes do digraph
    KosarajuSharirSCC scc = new KosarajuSharirSCC(G);

    // M é o número de componentes fortes no digraph
    // Separa os vértices do digraph dado em componentes fortemente conexas
    int M = scc.count();
    Queue<Integer>[] components = (Queue<Integer>[]) new Queue[M];
    for (int i = 0; i < M; i++)
      components[i] = new Queue<Integer>();
    for (int v = 0; v < G.V(); v++)
      components[scc.id(v)].enqueue(v);

    // Cria um vetor de booleanos
    // Se todo o vetor for true, então os vértices da componente de id M-1 são core vertices
    boolean[] isCore = new boolean[M];
    isCore[M-1] = true;
    // Pega um vértice de uma componente e marca todas as componentes que são alcançadas por esse vértice como true
    // Faz isso com todos os vértices de todas as componentes
    for (int i = M-1; i > 0; i--)
      for (int j : components[i])
        for (int k : G.adj(j))
          isCore[scc.id(k)] = true;

    // Se todo o vetor isCore[] for true, então há pelo menos um Core Vertice
    // Imprime o(s) core vertice(s)
    boolean flag = true;
    for (int i = 0; i < M-1 && flag; i++)
      if (!isCore[i]) flag = false;
    if (flag)
      for (int i : components[M-1])
        StdOut.print(i + " ");    
    StdOut.println();
  }
}
