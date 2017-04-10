/***************************************************************
* 
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Creative Problem 5.3.32 (Unique substrings; Algs4)
*
***************************************************************/

public class UniqueLRK
{
  // Dado um inteiro, retorna uma String de L digitos que representa o inteiro dado
  private static String buildPattern(int num, int L)
  {
    int aux = num;
    char[] patAux = new char[L];
    for (int i = L - 1; i > -1; i--)
    {
      patAux[i] = (char) ((aux % 10) + 48);
      aux /= 10;
    }
    String s = new String(patAux);
    return s;
  }

  public static void main(String[] args)
  {
/*
    long startTime;
    long stopTime;
    double deltaTime = 0;
    startTime = System.nanoTime();
*/
    int L = Integer.parseInt(args[0]);

    if (args.length > 1)
    {
      int N = Integer.parseInt(args[1]);

      // Gera a sequência de N digitos pseudo-aleatória
      String txt;
      char[] txt_aux = new char[N];
      for (int i = 0; i < N; i++)
        txt_aux[i] = (char) (StdRandom.uniform(10) + 48);
      txt = new String(txt_aux);

      if (L != 0)
      {
        int count = 0;
        int lim = (int) Math.pow(10, L);
        for (int l = 0; l < lim; l++)
        {
          String pat = buildPattern(l, L);
          RabinKarp RK = new RabinKarp(pat);
          int s = RK.search(txt);
          if (s != N) count++;      // Se a sub-sequência buscada existir na sequência, incrementa count em 1
        }
        StdOut.println(count);
      }
      else  // else if (L == 0)
      {
        int n = N;
        for (L = 0; n / 10 > 0; L++) n /= 10;
        while (true)
        {
          int l;
          int lim = (int) Math.pow(10, L);
          for (l = 0; l < lim; l++)
          {
            String pat = buildPattern(l, L);
            RabinKarp RK = new RabinKarp(pat);
            int s = RK.search(txt);
            if (s == N) break;  // Se alguma sub-sequência de L digitos não estiver na sequência de tamanho N, então essa sequência não é L-completa para o L testado
          }
          if (l != lim) L--;
          else          break;
        }
        StdOut.println(L);
      }
    }
    else  // else if (args.length == 1)
    {
      // Lê um texto da entrada padrão
      In in = new In();
      String txt = in.readAll();

      if (L != 0)
      {
        int count = 0;
        int lim = (int) Math.pow(10, L);
        for (int l = 0; l < lim; l++)
        {
          String pat = buildPattern(l, L);
          RabinKarp RK = new RabinKarp(pat);
          int s = RK.search(txt);
          if (s != txt.length()) count++;    // Se a sub-sequência buscada existir na sequência, incrementa count em 1
        }
        StdOut.println(count);
      }
      else  // else if (L == 0)
      {
        int n = txt.length();
        for (L = 0; n / 10 > 0; L++) n /= 10;
        while (true)
        {
          int l;
          int lim = (int) Math.pow(10, L);
          for (l = 0; l < lim; l++)
          {
            String pat = buildPattern(l, L);
            RabinKarp RK = new RabinKarp(pat);
            int s = RK.search(txt);
//            StdOut.println("L = " + L + " || l = " + l + " || s = " + s);
            if (s == txt.length()) break;  // Se alguma sub-sequência de L digitos não estiver na sequência de tamanho N, então essa sequência não é L-completa para o L testado
          }
          if (l != lim) L--;
          else          break;
        }
        StdOut.println(L);
      }
    }
/*
    stopTime = System.nanoTime ();
    deltaTime += (double) (stopTime - startTime) / 1000000000;
    StdOut.println ("delta time = " + deltaTime);
*/
  }
}
