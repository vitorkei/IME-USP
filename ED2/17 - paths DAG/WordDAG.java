/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vitor Kei Taira Tamada
* NUSP: 8516250
* Web Exercise 4.2.33 (Number of paths in a DAG; Algs4)
* 
***************************************************************/

public class WordDAG
{
  private Digraph dag;
  private IndexSET<String> words;

  // "cyclically shift the letters one position to the right"
  private String shift (String s)
  {
    return s.charAt(s.length() - 1) + s.substring(0, s.length() - 1);
  }

  // Retorna true se duas strings diferem uma da outra por apenas uma letra
  private boolean isNeighbor(String s, String t)
  {
    // Verifica se s é lexicograficamente maior que t
    if (s.compareTo(t) < 0) return false;

    // Verifica se a diferença do número de caracteres entre as duas palavras é, no máximo, 1
    // Se for maior que 1, as duas palavras com certeza não estão conectadas
    if (Math.abs(s.length() - t.length()) > 1) return false;

    if (s.length() != t.length())
    {
      // É garantido que os tamanhos de a e b são diferentes se chegou neste ponto;
      // logo, apenas essas duas verificações são o suficiente.
      // Duas palavras de tamanhos diferentes são conectadas se apenas a última letra é diferente. Portanto, preciso apenas do tamanho da menor palavra.
      int lim;
      if (s.length() < t.length()) lim = s.length();
      else                         lim = t.length();

      // Não há tolerância neste caso
      for (int i = 0; i < lim; i++)
        if (s.charAt(i) != t.charAt(i)) return false;
    }
    else
    {
      for (int i = 0; i < s.length() - 1; i++)
        if (s.charAt(i) != t.charAt(i)) return false;
    }

    return true;
  }

  // Gera um directed acyclic graph (DAG) à partir do argumento strings
  public WordDAG(String[] strings)
  {
    int size = strings.length;       // Número de elementos de strings[]
    // Inicializa um DAG vazio de tamanho igual ao comprimento do vetor de Strings strings
    dag = new Digraph (size);

    // Insere os elementos de strings[] em words, qualquer que seja sua ordem
    int longest = 0;                 // Tamanho da maior palavra do arquivo de entrada
    words = new IndexSET<String>();
    IndexSET<String> auxWords;
    for (int i = 0; i < size; i++)
    {
      if (strings[i].length() > longest)
        longest = strings[i].length();
      words.add(strings[i]);
    }

    for (int l = 0; l < longest; l++)
    {
      // (Re)inicia auxWords para receber as palavras "shiftadas"
      auxWords = new IndexSET<String>();
      // Adiciona as palavras da lista no IndexSET auxWords na ordem que são lidas
      for (int j = 0; j < size; j++)
      {
        String temp = words.keyOf(j);
        // Se não for a primeira iteração, faz shift para a direita um número de vezes igual ao número da iteração menos 1 (2ª iteração faz 1 shift, 3ª iteração faz 2 shifts, ...)
        for (int l_aux = 0; l_aux < l; l_aux++)
          temp = shift(temp);
        auxWords.add(temp);
      }

      // Recebe as palavras da lista em um array para poder fazer a verificação de adjacência
      // As palavras são recebidas em ordem alfabética
      // Palavras adjacentes por diferença de tamanho tem prioridade maior do que por tamanho igual e letra diferente (e.g.: as palavras 'porta', 'porto' e 'portal' vem na ordem 'porta', 'portal', 'porto')
      int k = 0;
      String[] sortedWords = new String[size];
      for (String s : auxWords.keys())
      {
        sortedWords[k] = s;
//        StdOut.print(s + " ");
        k++;
      }
//      StdOut.println("\n-----");

      // Verifica se uma palavra da lista está conectada à palavra anterior
      // Quando as palavras estão ordenadas em ordem crescente lexicograficamente, garante-se que a palavra na posição 0 é a de menor valor lexicografico enquanto a na última posição é a de maior valor
      // Ou seja, é certeza que sortedWords[i] é lexicograficamente maior que sortedWords[i-1]
      for (int i = size - 1; i > 0; i--)
      {
        boolean flag = true;
        // Se uma palavra s_i está conectada à s_(i-1), verifica se s_i está conectada com s_(i-2), s_(i-3), ..., até que isNeighbor() retorne false
        // Isso garante que, por exemplo, haja um arco saindo de 'white' e indo para 'while' e outro para 'whine'
        for (int i_minus = 1; flag && i - i_minus > -1; i_minus++)
        {
          String word1 = sortedWords[i];
          String word2 = sortedWords[i - i_minus];
//          StdOut.println("i = " + i + "\nword1 = " + word1 + "\ni - i_minus = " + (i - i_minus) + "\nword2 = " + word2);

          if (word1.compareTo(word2) > 0 && isNeighbor(word1, word2))
          {
            int index1 = auxWords.indexOf(word1);
            int index2 = auxWords.indexOf(word2);
            String words1 = words.keyOf(index1);
            String words2 = words.keyOf(index2);
//            StdOut.println(words1 + "\n" + words2 + "\n");
            dag.addEdge(words.indexOf(words1), words.indexOf(words2));
          }
          else flag = false;
//          StdOut.println();
        }
      }
    }
  }

  // Imprime as arestas do DAG
  public void PrintDag()
  {
    Out out = new Out("saida.txt");

    for (int from = 0; from < dag.V(); from++)
    {
      String word1 = words.keyOf(from);
      for (int to : dag.adj(from))
      {
        String word2 = words.keyOf(to);

        out.print(word1 + " " + word2 + "\n");
      }
    }

    out.close();
  }

  // Imprime a quantidade de caminhos distintos entre a e b no DAG
  public void PrintPathCount(String a, String b)
  {
    Out out = new Out("saida.txt");

    int size = dag.V();

    String[] color     = new String[size];
    int[] num_paths    = new int[size];

    for (int i = 0; i < dag.V(); i++)
    {
      color[i] = "WHITE";
      num_paths[i] = 0;
    }

    int from = words.indexOf(a);
    int to   = words.indexOf(b);
    num_paths[to] = 1;

    Stack<Integer> path = new Stack<Integer>();
    path.push(from);
    while (!path.isEmpty())
    {
      int c = path.pop();
      StdOut.println("c = " + c + "\nfrom = " + from);
      if (c == from)
      {
        if (color[c].equals("WHITE"))
        {
          StdOut.println("teste c == from");
          color[c] = "GRAY";
          path.push(c);
          for (int aux : dag.adj(c))
            path.push(aux);
        }
      }
      else // c == to
      {
        StdOut.println("teste c == to");
        color[c] = "BLACK";
        for (int aux : dag.adj(c))
          num_paths[c] += num_paths[aux];
      }
    }

    out.print(num_paths[from]);

    out.close();
  }

  public static void main(String[] args)
  {
    String[] test = {"cobra", "sobra", "sobrar", "cobro", "cobrou", "sobrando", "cobre"};
    Merge.sort(test);
/*
    for (int i = 0; i < test.length; i++)
      StdOut.print(test[i] + " ");
    StdOut.println("\n*****");
*/
    WordDAG dag = new WordDAG(test);
    StdOut.println("PrintDag() ou PrintPathCount()?");
    int choice = StdIn.readInt();
    if (choice == 0)
      dag.PrintDag();
    else if (choice == 1)
      dag.PrintPathCount("cobro", "cobra");
    else
      StdOut.println("Opção inválida. Encerrando programa");
  }
}
