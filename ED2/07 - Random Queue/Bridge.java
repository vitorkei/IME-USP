/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Creative Problem 1.3.35 (Random queue; Algs4)
*
***************************************************************/

public class Bridge
{
  private int suit;  // 0 = paus (C); 1 = ouros (D); 2 = copas (H); 3 = espadas (S)
  private int rank;  // 2, 3, ..., rei, A = 0, 1, ..., 11, 12

  // Cria uma nova carta baseada em inteiros: 0 = 2 de paus, 1 = 3 de paus, ..., 51 = A de espadas
  public Bridge (int card)
  {
    rank = card % 13;
    suit = card / 13;
  }

  // Retorna o naipe e o valor da carta respectivamente
  public int suit () { return suit; }
  public int rank () { return rank; }

  // Ordena as cartas, primeiramente, por naipe e, em segundo, por valor
  public boolean less (Bridge c)
  {
    if (suit < c.suit)
      return true;

    else if (suit > c.suit)
      return false;

    else if (rank < c.rank)
      return true;

    else
      return false;
  }

  // Imprime o valor e naipe da carta como uma string
  public String toString ()
  {
    String ranks = "23456789TJQKA";
    String suits = "CDHS";
    return ranks.charAt (rank) + "" + suits.charAt (suit);
  }

  public static void main (String[] args)
  {
    RandomQueue<Integer> rq = new RandomQueue<Integer> ();

    // Insere as cartas na RandomQueue
    for (int i = 0; i < 52; i++)
      rq.enqueue (i);

    // Saca uma mão com 13 cartas escolhidas aleatoriamente
    for (int i = 0; i < 13; i++)
    {
      int aux = rq.dequeue ();
      Bridge card = new Bridge (aux);
      StdOut.println (card.toString ());
    }
  }
}
