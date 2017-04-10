/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vitor Kei Taira Tamada
* NUSP: 8516250
* Web Exercise 4.1.12 (Faster word ladders; Algs4)
* 
* O programa utiliza 2 IndexSET<String>, words e auxWords, e um
*   vetor String[words.size()] sortedWords
* words serve para colocar as palavras no grafo, auxWords para receber
*   as palavras "shiftadas", e sortedWords receber as palavras
*   "shiftadas" ordenadas para fazer as verificações de adjacência
* 
***************************************************************/

public class FasterWordLadders
{
  // "cyclically shift the letters one position to the right"
  public static String shift (String s)
  {
    return s.charAt(s.length() - 1) + s.substring(0, s.length() - 1);
  }

  // Retorna true se duas strings diferem uma da outra por apenas uma letra
  public static boolean isNeighbor(String a, String b)
  {
    // Verifica se a diferença do número de caracteres entre as duas palavras é, no máximo, 1
    // Se for maior que 1, as duas palavras com certeza não estão conectadas
    if (Math.abs(a.length() - b.length()) > 1) return false;

    if (a.length() != b.length())
    {
      // É garantido que os tamanhos de a e b são diferentes se chegou neste ponto;
      // logo, apenas essas duas verificações são o suficiente.
      // Duas palavras de tamanhos diferentes são conectadas se apenas a última letra é diferente. Portanto, preciso apenas do tamanho da menor palavra.
      int lim;
      if (a.length() < b.length()) lim = a.length();
      else                         lim = b.length();

      // Não há tolerância neste caso
      for (int i = 0; i < lim; i++)
        if (a.charAt(i) != b.charAt(i)) return false;
    }
    else
    {
      // Verifica se todas as letras além da última são iguais
      // Se alguma for diferente, retorna false. Desta forma, liga duas palavras de mesmo tamanho apenas se somente a última letra for diferente
      for (int i = 0; i < a.length() - 1; i++)
        if (a.charAt(i) != b.charAt(i)) return false;
    }

    return true;
  }

  public static void main(String[] args)
  {
    // Lê uma lista de strings e insere-as em words, qualquer que seja sua ordem
    int longest = 0;  // Tamanho da maior palavra do arquivo de entrada
    In in = new In(args[0]);
    IndexSET<String> words = new IndexSET<String>();
    IndexSET<String> auxWords = new IndexSET<String>();
    while (!in.isEmpty())
    {
      String word = in.readString();
      if (word.length() > longest)
        longest = word.length();
      words.add(word);
    }

    // Insere conexões entre palavras vizinhas em um grafo;
    Graph G = new Graph(words.size());
    for (int l = 0; l < longest; l++)
    {
      // Adiciona as palavras da lista em um IndexSET na ordem que são lidas
      for (int j = 0; j < words.size(); j++)
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
      String[] sortedWords = new String[auxWords.size()];
      for (String s : auxWords.keys())
      {
        sortedWords[k] = s;
        k++;
      }

      // Verifica se uma palavra da lista está conectada à palavra seguinte
      for (int i = 0; i < words.size() - 1; i++)
      {
        boolean flag = true;
        // Se uma palavra s_i está conectada à s_(i+1), verifica se s_i está conectada com s_(i+2), s_(i+3), ..., até que isNeighbor() retorne false.
        // Isso faz com que, por exemplo, 'whine', 'white' e 'while' sejam todas conectadas entre si
        for (int i_plus = 1; flag && i + i_plus < words.size(); i_plus++)
        {
          String word1 = sortedWords[i];
          String word2 = sortedWords[i + i_plus];

          if (isNeighbor(word1, word2))
          {
            int index1 = auxWords.indexOf(word1);
            int index2 = auxWords.indexOf(word2);
            String words1 = words.keyOf(index1);
            String words2 = words.keyOf(index2);
            G.addEdge(words.indexOf(words1), words.indexOf(words2));
          }
          else flag = false;
        }
      }

      // Reinicia o auxWords para que receba as palavras "shiftadas para a direita" no começo da iteração
      auxWords = new IndexSET<String>();
    }

    // Roda Breadth first search (BFS)
    while (!StdIn.isEmpty())
    {
      String from = StdIn.readString();
      String to   = StdIn.readString();

      if (!words.contains(from)) throw new RuntimeException(from + "não faz parte da lista de palavras");
      if (!words.contains(to))   throw new RuntimeException(to + "não faz parte da lista de palavras");

      BreadthFirstPaths bfs = new BreadthFirstPaths(G, words.indexOf(from));

      if (bfs.hasPathTo(words.indexOf(to)))
      {
        StdOut.println(bfs.distTo(words.indexOf(to)));
        for (int v : bfs.pathTo(words.indexOf(to)))
          StdOut.println(words.keyOf(v));
      }
      else StdOut.println("NOT CONNECTED");
      StdOut.println();
    }
  }
}
