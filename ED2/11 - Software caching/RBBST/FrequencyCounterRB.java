/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Creative Problem 3.3.30 (Software caching; Algs4)
*
***************************************************************/

public class FrequencyCounterRB
{
  // Não instancia
  private FrequencyCounterRB ()  {}

  public static void main (String[] args)
  {
    long startTime = System.nanoTime ();

    int distinct = 0, words = 0;
    int minlen = Integer.parseInt (args[0]);
    RedBlackBST<String, Integer> rbbst = new RedBlackBST<String, Integer> ();

    // Computa a frequência de cada palavra
    while (!StdIn.isEmpty ())
    {
      String key = StdIn.readString ();
      if (key.length () < minlen) continue;
      words++;
      if (rbbst.contains (key))
        rbbst.put (key, rbbst.get (key) + 1);
      else
      {
        rbbst.put (key, 1);
        distinct++;
      }
    }

    // Encontra a chave com a maior frequência
    String max = "";
    rbbst.put (max, 0);
    for (String word : rbbst.keys ())
      if (rbbst.get (word) > rbbst.get (max))
        max = word;

    StdOut.println (max + " " + rbbst.get (max));
    StdOut.println ("distinct = " + distinct);
    StdOut.println ("words    = " + words);

    long stopTime = System.nanoTime ();
    double deltaTime = (double) (stopTime - startTime) / 1000000000;
    StdOut.println ("delta time = " + deltaTime);
  }
}
