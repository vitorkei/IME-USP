/***************************************************************
* 
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Exercise 5.2.2 (Unique substrings of length L; Algs4)
*
***************************************************************/

public class UniqueL
{
  public static void main(String[] args)
  {
    long startTime;
    long stopTime;
    double deltaTime;
    startTime = System.nanoTime();

    int L = Integer.parseInt(args[0]);

    TST<Integer> st = new TST<Integer>();

    // Se há um argumento N de entrada
    if (args.length > 1)
    {
      
      int N = Integer.parseInt(args[1]);

      // Gera uma sequência de N digitos gerados pseudo-aleatoriamente
      String nums;
      char[] nums_aux = new char[N];
      for (int i = 0; i < N; i++)
      {
        int a = StdRandom.uniform(10) + 48;
        nums_aux[i] = (char) a;
      }
      nums = new String(nums_aux);
      // Se L for diferente de 0,
      // imprime o número de substrings diferentes de tamanho L
      if (L != 0)
      {
        for (int i = 0; i + L <= N; i++)
        {
          String s = nums.substring(i, i + L);
          st.put(s, i);
          if (st.size() == Math.pow(10, L)) break;
        }
        StdOut.println(st.size());
      }
      // Se L for igual a zero,
      // imprime o maior valor de L tal que a sequência é L-completa.
      else
      {
        boolean flag = false;
        int n = N;
        for (L = 0; n / 10 > 0; L++) n /= 10;
        while (st.size() != Math.pow(10, L))
        {
          st = new TST<Integer>();
          for (int i = 0; i + L <= N; i++)
          {
            String s = nums.substring(i, i + L);
            st.put(s, i);
          }
          // Verifica se L+1 faz a sequência ser L-completa
          if (!flag && st.size() == Math.pow(10, L)) L++;
          if (!flag && st.size() != Math.pow(10, L))
          {
            L--;
            flag = true;
          }
          if (L == 0) break;
        }
        StdOut.println(L);
      }
    }

    // Se NÃO há um argumento N de entrada
    else
    {
      // Lê um texto da entrada padrão
      In in = new In();
      String text = in.readAll();

      // Se L for diferente de 0,
      // imprime o número de substrings diferentes de tamanho L
      if (L > 0)
      {
        for (int i = 0; i + L <= text.length(); i++)
        {
          String s = text.substring(i, i + L);
          st.put(s, i);
        }

        StdOut.println(st.size());
      }
      // Se L for igual a zero
      // imprime o maior valor de L tal que a sequência é L-completa. É garantido que a entrada é composta apenas de digitos.
      else
      {
        int n = text.length();
        for (L = 0; n / 10 > 0; L++) n /= 10;
        while (st.size() != Math.pow(10, L))
        {
          st = new TST<Integer>();
          for (int i = 0; i + L <= text.length(); i++)
          {
            String s = text.substring(i, i + L);
            st.put(s, i);
          }
          // Verifica se L+1 faz a sequência ser L-completa
          if (st.size() != Math.pow(10, L)) L--;
          if (L == 0) break;
        }
        StdOut.println(L);
      }
    }
    stopTime = System.nanoTime ();
    deltaTime = (double) (stopTime - startTime) / 1000000000;
    StdOut.println ("delta time = " + deltaTime);
  }
}

