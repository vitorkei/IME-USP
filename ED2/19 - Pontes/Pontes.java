/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Detecção de pontes em grafos
*
***************************************************************/

public class Pontes
{
  private boolean[] marked; // marked[v] == true significa que o vértice já foi visitado durante a construção do digraph no método dfs()
  private int[] height;     // height[v] == altura do vértice v na árvore
  private Digraph dg;       // Digraph criado pelo one-way street theorem

  // Cria um digraph usando o vértice 0 como raíz e utilizando o One-way street theorem
  public Pontes(Graph G)
  {
    int size = G.V();

    marked = new boolean[size];
    height = new int[size];
    dg = new Digraph(size);
    for (int i = 0; i < size; i++)
      height[i] = 0;

    dfs(G, 0, 0, 0);
  }

  // int v == vértice sendo visitado
  // int prev == previous vertex | impede que um vértice crie uma aresta direcionada para o vértice anterior
  // cHeight == current height   | altura atual do vértice visitado
  private void dfs (Graph G, int v, int prev, int cHeight)
  {
    marked[v] = true;
    height[v] = cHeight;
    for (int w : G.adj(v))
    {
      if (!marked[w])
      {
        dg.addEdge(v, w);
        dfs(G, w, v, cHeight + 1);
      }
      // Aponta para um vértice que já foi visitado caso haja uma aresta entre os dois
      // É garantido que a altura de w é menor que a de v; logo, a aresta v->w está "apontando para cima"
      else if (w != prev)
      {
        if (height[v] > height[w])
          dg.addEdge(v, w);
      }
    }
  }

  // Imprime as pontes do grafo caso existam
  public void bridges()
  {
    KosarajuSharirSCC scc = new KosarajuSharirSCC(dg);

    // Separa os vértices do digraph dado em componentes fortemente conexas
    int M = scc.count();
    Queue<Integer>[] components = (Queue<Integer>[]) new Queue[M];
    for (int i = 0; i < M; i++)
      components[i] = new Queue<Integer>();
    for (int v = 0; v < dg.V(); v++)
      components[scc.id(v)].enqueue(v);

    // Organiza as componentes fortemente conexas pelo método de KosarajuSharir
    // Se uma aresta sair de um vértice de uma componente fortemente conexa e for para um vértice de outra componente, então essa aresta é uma ponte
    for (int i = M-1; i > 0; i--)
      for (int j : components[i])
        for (int k : dg.adj(j))
          if (scc.id(j) != scc.id(k))
            StdOut.println(j + " " + k);
  }

  public static void main(String[] args)
  {
    int n = StdIn.readInt();
    int m = StdIn.readInt();
    Graph G = new Graph(n);
    Pontes P;

    // Constrói o grafo não direcionado
    for (int i = 0; i < m; i++)
    {
      int a = StdIn.readInt();
      int b = StdIn.readInt();

      G.addEdge(a, b);
    }

    // Constroi o grafo direcionado segundo o One-way street theorem
    P = new Pontes(G);
    // Imprime as pontes do grafo
    P.bridges();
  }
}
