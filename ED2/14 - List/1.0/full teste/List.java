/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vitor Kei Taira Tamada
* NUSP: 8516250
* Creative Problem 3.5.27 (List; Algs4)
*
***************************************************************/

import java.util.Iterator;
import java.util.NoSuchElementException;

public class List<Item extends Comparable<Item>> implements Iterable<Item>
{
  private ST<Integer, Item> int_item;
  private ST<Item, Integer> item_int;

  // Inicializa duas tabelas de símbolo vazias
  public List()
  {
    int_item = new ST<Integer, Item>();
    item_int = new ST<Item, Integer>();
  }

  // Adiciona um item ao começo da lista
  public void addFront(Item item)
  {
    int i;
    if (int_item.isEmpty()) i = 0;
    else                    i = int_item.min() - 1;

    int_item.put(i, item);
    item_int.put(item, i);
  }

  // Adiciona um item ao final da lista
  public void addBack(Item item)
  {
    int i;
    if (int_item.isEmpty()) i = 0;
    else                    i = int_item.max() + 1;

    int_item.put(i, item);
    item_int.put(item, i);
  }

  // Remove e retorna o item do começo da lista
  public Item deleteFront()
  {
    if (int_item.isEmpty()) throw new NoSuchElementException("Chamou o método deleteFront() com uma lista vazia");

    int i = int_item.min();
    Item aux = int_item.get(i);

    int_item.delete(i);
    item_int.delete(aux);
    return aux;
  }

  // Remove e retorna o item do final da lista
  public Item deleteBack()
  {
    if (int_item.isEmpty()) throw new NoSuchElementException("Chamou o método deleteBack() com uma lista vazia");

    int i = int_item.max();
    Item aux = int_item.get(i);

    int_item.delete(i);
    item_int.delete(aux);
    return aux;    
  }

  // Deleta item da lista
  public void delete(Item item)
  {
    if (int_item.isEmpty()) throw new NoSuchElementException("Chamou o método void delete() com uma lista vazia");
    int i = item_int.get(item);

    int_item.delete(i);
    item_int.delete(item);
  }

  /* Precisa de ajustes */
  // Adiciona item na i-ésima posição da lista
  public void add(int i, Item item)
  {
    if (!int_item.isEmpty())
    {
      if (!int_item.contains(i))
      {
        int i_aux = int_item.min();
        if (i_aux != 0) i_aux += i;
        else            i_aux  = i;
        int_item.put(i_aux, item);
        item_int.put(item, i_aux);
      }
      else
      {
        
      }
    }
  }

  // Deleta o i-ésimo item da lista
  public Item delete(int i)
  {
    if (int_item.isEmpty()) throw new NoSuchElementException("Chamou o método Item delete() com uma lista vazia");

    // A lista não vai de 0 até n necessariamente, mas de m até n, sendo que m pode ser um número negativo
    // A checagen abaixo verifica se a menor chave é negativa e executa o método de acordo
    int i_aux = int_item.min();
    if (i_aux != 0) i_aux += i;
    else            i_aux  = i;
//    StdOut.println ("Item delete(int " + i + ")\ni_aux = " + i_aux + "\n");
    Item aux = int_item.get(i_aux);
    int_item.delete(i_aux);
    item_int.delete(aux);
    return aux;
  }

  // Retorna o i-ésimo elemento da lista
  public Item get(int i)
  {
    int i_aux = int_item.min();
    if (i_aux != 0) i_aux += i;
    else            i_aux  = i;

    return int_item.get(i_aux);
  }

  // Verifica se a lista está vazia
  public boolean isEmpty()           { return int_item.isEmpty();      }

  // Retorna o número de itens na lista
  public int size()                  { return int_item.size();         }

  // Verifica se o item está na lista
  public boolean contains(Item item) { return item_int.contains(item); }

  // Iterador
  public Iterator<Item> iterator() { return new ListIterator(); }
  private class ListIterator implements Iterator<Item>
  {
    private int i = int_item.min();
    private int max = int_item.max();
    public boolean hasNext() { return i <= max;                           }
    public void remove()     { throw new UnsupportedOperationException(); }

    public Item next()
    {
      if (!hasNext())
        throw new NoSuchElementException();

      while (!int_item.contains(i)) i++;
      Item aux = int_item.get(i);
      i++;
      return aux;
    }
  }

  // Unit test
  public static void main(String[] args)
  {
    String[] jojo = new String[6];
    jojo[0] = "SONO";
    jojo[1] = "CHI";
    jojo[2] = "NO";
    jojo[3] = "SADAME";
    jojo[4] = "JOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    jojo[5] = "JO";

    StdOut.println("\n=|=|=|=| Bloco de testes ALPHA - menor chave menor que zero |=|=|=|=\n");
    StdOut.println("Teste 1\nCriação de List vazia, isEmpty()\n");

    List<String> list = new List<String>();
    StdOut.println("isEmpty() = " + list.isEmpty());

/********** Separação de bloco de testes **********/

    StdOut.println("\n********************\nTeste 2\naddFront(), addBack(), iterator(), size()\n");

    list.addFront(jojo[2]);
    StdOut.print("addFront() - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\nsize - " + list.size());

    list.addFront(jojo[1]);
    StdOut.print("addFront() - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\nsize - " + list.size());

    list.addFront(jojo[0]);
    StdOut.print("addFront() - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\nsize - " + list.size());

    list.addBack(jojo[3]);
    StdOut.print("addBack()  - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\nsize - " + list.size());

    list.addBack(jojo[4]);
    StdOut.print("addBack()  - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\nsize - " + list.size());

    list.addBack(jojo[5]);
    StdOut.print("addBack()  - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\nsize - " + list.size());

/********** Separação de bloco de testes **********/

    StdOut.println("\n********************\nTeste 3\ndeleteFront(), deleteBack(), void delete(Item), size()\n");

    StdOut.print("deleteFront() - " + list.deleteFront() + "\nResultado - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    StdOut.print("deleteBack() - " + list.deleteBack() + "\nResultado - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    StdOut.print("list.delete('NO')\nResultado - ");
    list.delete("NO");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println();

    StdOut.println("\nsize() - " + list.size() + "\n");

    // Retomando a forma original da lista (chaves pertencem ao intervalo [-2, 3]
    list.deleteFront();
    list.addFront(jojo[2]);
    list.addFront(jojo[1]);
    list.addFront(jojo[0]);
    list.addBack(jojo[5]);
    StdOut.println("Original:");
    for (String s : list)
      StdOut.print(s + " ");

/********** Separação de bloco de testes **********/

    StdOut.println("\n********************\nTeste 4\nItem delete(int), get(), contains(), isEmpty()\n");

    StdOut.print("list.delete(4) - " + list.delete(4) + "\nResultado - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    StdOut.println("list.get(2) - " + list.get(2));
    StdOut.println("list.get(4) - " + list.get(4) + "\n");

    StdOut.println("list.contains('JOOOOOOOOOOOOOOOOOOOOOOOOOOO') - " + list.contains(jojo[4]));
    StdOut.println("list.contains('SADAME') - " + list.contains("SADAME"));
    StdOut.println("list.contains('WRYYYY') - " + list.contains("WRYYYY"));
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    StdOut.println("isEmpty() - " + list.isEmpty() + "\n");

    // Retomando a forma original da lista
    StdOut.println("Original:");
    list.deleteBack();
    list.addBack(jojo[4]);
    list.addBack(jojo[5]);
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

/********** Separação de bloco de testes **********/

    StdOut.println("|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|\n|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|\n|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|");
    StdOut.println("\n=|=|=|=| Bloco de testes BETA - menor chave igual a zero |=|=|=|=");
    StdOut.println("\nTeste 1\ndeleteFront(), deleteBack(), delete(Item), addFront(), addBack()\n");

    // Reinicia a lista com menor chave igual a zero
    for (String s : list)
      list.delete(s);
    for (int i = 0; i < 6; i++)
      list.addBack(jojo[i]);

    StdOut.print("deleteFront() - " + list.deleteFront() + "\nResultado - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    StdOut.print("deleteBack() - " + list.deleteBack() + "\nResultado - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    StdOut.print("list.delete('CHI')\nResultado - ");
    list.delete("CHI");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    // Retomando a forma original da lista e rodando mais uns testes
    StdOut.print("addFront() - ");
    list.addFront(jojo[1]);
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println();

    StdOut.print("addFront() - ");
    list.addFront(jojo[0]);
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println();

    StdOut.print("addBack() - ");
    list.addBack(jojo[5]);
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println();

    StdOut.println("\nOriginal:");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println();

/********** Separação de bloco de testes **********/

    StdOut.println("\n********************\nTeste 2\nItem delete(int), get(), contains()\n");

    StdOut.println("Item delete(3) - " + list.delete(3));
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    StdOut.println("list.get(5) - " + list.get(5));
    StdOut.println("list.get(3) - " + list.get(3));

    StdOut.println("\nlist.contains('SONO') - " + list.contains("SONO"));
    StdOut.println("list.contains('SADAME') - " + list.contains(jojo[3]));
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    StdOut.println("iteração por for com i inicial igual a 0");
    for (int i = 0; i < 6; i++)
      StdOut.print(list.get(i) + " ");
    StdOut.println("\n");

/********** Separação de bloco de testes **********/

    StdOut.println("|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|\n|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|\n|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|");

    StdOut.println("\n=|=|=|=| Bloco de testes GAMA - menor chave maior que zero |=|=|=|=");
    StdOut.println("\nTeste 1\ndeleteFront(), deleteBack(), delete(Item), addFront(), addBack()\n");

    // Reinicia a lista com menor chave maior que zero
    for (String s : list)
      list.delete(s);
    list.addBack("MIRAI");
    list.addBack("NI");
    list.addBack("KOUUN");
    list.addBack("WO");
    for (int i = 0; i < 6; i++)
      list.addBack(jojo[i]);
    for (int i = 0; i < 10; i++)
      StdOut.print(list.get(i) + " ");
    StdOut.println("\n");
    for (int i = 0; i < 4; i++)
      list.deleteFront();
    for (int i = 0; i < 10; i++)
      StdOut.print(list.get(i) + " ");
    StdOut.println("\n");

    StdOut.print("deleteFront() - " + list.deleteFront() + "\nResultado - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    StdOut.print("deleteBack() - " + list.deleteBack() + "\nResultado - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    StdOut.print("list.delete('CHI')\nResultado - ");
    list.delete("CHI");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    // Retomando a forma original da lista e rodando mais uns testes
    StdOut.print("addFront() - ");
    list.addFront(jojo[1]);
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println();

    StdOut.print("addFront() - ");
    list.addFront(jojo[0]);
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println();

    StdOut.print("addBack() - ");
    list.addBack(jojo[5]);
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println();

    StdOut.println("\nOriginal:");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println();

/********** Separação de bloco de testes **********/

    StdOut.println("\n********************\nTeste 2\nItem delete(int), get(), contains()\n");

    StdOut.println("Item delete(3) - " + list.delete(3));
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    StdOut.println("list.get(5) - " + list.get(5));
    StdOut.println("list.get(3) - " + list.get(3));

    StdOut.println("\nlist.contains('SONO') - " + list.contains("SONO"));
    StdOut.println("list.contains('SADAME') - " + list.contains(jojo[3]));
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    for (int i = 0; i < 10; i++)
      StdOut.print(list.get(i) + " ");
    StdOut.println("\n");
  }
}
