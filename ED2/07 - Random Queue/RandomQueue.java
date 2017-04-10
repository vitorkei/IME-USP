/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Creative Problem 1.3.35 (Random queue; Algs4)
*
***************************************************************/

import java.util.NoSuchElementException;

public class RandomQueue<Item>
{
  private Item[] rq;    // rq = random queue
  private int N;        // Número de elementos na fila
  private int first;    // Índice do primeiro elemento da fila
  private int last;     // Índice do próximo espaço disponível

  // Inicializa uma fila vazia
  public RandomQueue ()
  {
    rq = (Item[]) new Object[2];
    N = 0;
    first = 0;
    last = 0;
  }

  // Verifica se a fila está vazia, retornando true em caso afirmativo e false caso contrário
  public boolean isEmpty () { return N == 0; }

  // Retorna o número de elementos na fila
  public int size () { return N; }

  // Ajusta o tamanho do array
  private void resize (int max)
  {
    assert max >= N;
    Item[] temp = (Item[]) new Object[max];
    for (int i = 0; i < N; i++)
      temp[i] = rq[(first + i) % rq.length];

    rq = temp;
    first = 0;
    last = N;
  }

  // Adiciona um elemento à fila
  public void enqueue (Item item)
  {
    // Dobra o tamanho da RandomQueue caso seu espaço atual não seja suficiente
    if (N == rq.length)
      resize(2 * rq.length);

    rq[last++] = item;  // Adiciona um item ao final da fila
    N++;                // Incrementa a variável que conta o número de elementos da fila
  }

  // Remove e retorna um elemento da fila escolhido aleatoriamente
  public Item dequeue ()
  {
    // Verifica se a fila está vazia
    if (isEmpty ())
      throw new NoSuchElementException ("Fila vazia - não é possível remover e retornar um item da fila");

    int out = StdRandom.uniform (last);
    Item item = rq[out];
    rq[out] = rq[last - 1];
    rq[last - 1] = null;
    N--;
    last--;

    // Reduz o tamanho da fila caso necessário
    if (N > 0 && N == rq.length/4)
      resize (rq.length/2);

    return item;
  }

  // Retorna um item da fila escolhido aleatoriamente
  // Não há remoção desse item da fila
  public Item sample ()
  {
    // Verifica se a fila está vazia
    if (isEmpty ())
      throw new NoSuchElementException ("Fila vazia - não é possível retornar um item da fila");

    int out = StdRandom.uniform (last);

    return rq[out];
  }

  public static void main (String[] args)
  {
    RandomQueue<String> rq = new RandomQueue<String> ();
    while (!StdIn.isEmpty ())
    {
      String item = StdIn.readString ();
      if (!item.equals ("-"))
        rq.enqueue (item);
      else if (!rq.isEmpty ())
        StdOut.print (rq.dequeue () + " ");
    }

    StdOut.println ("(" + rq.size () + " left on queue)");
  }
}
