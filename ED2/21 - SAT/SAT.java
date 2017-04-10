/***************************************************************
* 
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Creative Problem 4.2.26 (2-satisfiability; Algs4)
*
***************************************************************/

public class SAT
{
  public static void main(String[] args)
  {
    int n = StdIn.readInt();
    int m = StdIn.readInt();

    // Construção de um digraph segundo a sugestão do enunciado
    Digraph SAT = new Digraph(2*n);
    for (int i = 0; i < m; i++)
    {
      // Como os vértices de um grafo pertencem ao intervalo [0, n-1], os primeiros n vértices correspondem à negação do literal
      // enquanto os n últimos vértices correspondem ao próprio literal
      //        literal <-> índice no digraph
      // e.g.:  n = 3 => SAT.size() = 6
      //       -1 <-> 0, -2 <-> 1, -3 <-> 2
      //        1 <-> 3,  2 <-> 4,  3 <-> 5
      
      int x = StdIn.readInt();
      int nx;
      if (x > 0)
      {
        x = n + x - 1;  // x
        nx = -n + x;    // x'
      }
      else
      {
        x = -x - 1;     // x
        nx = n + x;     // x'
      }

      int y = StdIn.readInt();
      int ny;
      if (y > 0)
      {
        y = n + y - 1;  // y
        ny = -n + y;    // y'
      }
      else
      {
        y = -y - 1;     // y
        ny = n + y;     // y'
      }

      SAT.addEdge(ny, x); // y' -> x
      SAT.addEdge(nx, y); // x' -> y
    }

    // Separa os vértices do digraph em componentes fortemente conexas
    KosarajuSharirSCC scc = new KosarajuSharirSCC(SAT);
    boolean flag = true; // Se flag == false, então a saída do programa será "MENTIRA"

    // Verifica se um literal e a sua negação estão na mesma componente fortemente conexa
    for (int i = 0; i < n; i++)
      // Seja i o índice um vértice do digraph, v_i = v'_(i + n)
      if (scc.stronglyConnected(i, i + n))
      {
        StdOut.println("MENTIRA");
        flag = false;
        break;
      }

    if (flag)
    {
      StdOut.println("VERDADE");
      boolean[] marked = new boolean[2*n];  // Array de booleanos para marcar quais vértices já tiveram suas adjacências visitadas
      int[] valor = new int[n];             // Valoração dos literais
      for (int i = 0; i < n; i++)
        valor[i] = 0;
      int count = scc.count();

      // Forma um vetor de filas em que cada fila (elemento do vetor) é uma componente fortemente conexa e cada item dessa fila é um vértice pertencente a componente em questão
      Queue<Integer>[] components = (Queue<Integer>[]) new Queue[count];
      for (int i = 0; i < count; i++)
        components[i] = new Queue<Integer>();
      for (int v = 0; v < SAT.V(); v++)
        components[scc.id(v)].enqueue(v);

      // Passa por todos os vértices do digraph pela ordem do topological sort
      for (int i = count-1; i > -1; i--)  // Percorre cada componente, começando pela que tem inDegree == 0
        for (int j : components[i])       // Percorre os vértices da componente i
        {
          marked[j] = true;

          // Converte o índice do vértice visitado para o seu valor correspondente se necessário
          // e.g.: no exemplo acima, vértice de índice 3 se torna 0, de índice 4 se torna 1 e de índice 5 se torna 2;
          //       cada um correspondendo a um número (0 => 1, 1 => 2, 2 => 3) para ser usado em valor[]
          int literal;
          if (j >= n)
          {
            literal = j - n;
            valor[literal] = 1;
          }
          else
            valor[j] = 0;
        }

      // Imprime um conjunto de valorações que tornam a expressão verdadeira
      for (int i = 0; i < n; i++)
        StdOut.print(valor[i] + " ");
      StdOut.println();

    }
  }
}
