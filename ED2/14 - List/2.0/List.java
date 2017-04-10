/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vitor Kei Taira Tamada
* NUSP: 8516250
* Creative Problem 3.5.27 (List; Algs4)
* 
* Dependencies:
* -RedBlackBST.java
* -StdOut.java
*
***************************************************************/

import java.util.Iterator;
import java.util.NoSuchElementException;

public class List<Item extends Comparable<Item>> implements Iterable<Item>
{
  // A chave da árvore d_item (double) pertence ao intervalo (0, 1)
  private RedBlackBST<Double, Item>    d_item;
  private RedBlackBST<Item, Double>    item_d;

  // Inicializa duas tabelas de símbolos vazias
  public List()
  {
    d_item = new RedBlackBST<Double, Item>();
    item_d = new RedBlackBST<Item, Double>();
  }

  // Adiciona um item ao começo da lista
  // 4*log(n) ~ log(n)
  public void addFront(Item item)
  {
    double d;
    int i;
    if (d_item.isEmpty()) d = 0.5;
    else                  d = d_item.min() / 2; // [min() - 0]/2 garante que será a menor possível e, ainda sim, maior que zero

    d_item.put(d, item);
    item_d.put(item, d);
  }

  // Adiciona um item ao final da lista
  // 5*log(n) ~ log(n)
  public void addBack(Item item)
  {
    double d;
    if (d_item.isEmpty()) d = 0.5;
    else                  d = (1.0 - d_item.max()) / 2 + d_item.max(); // Garante que será a maior possível e, ainda sim, menor que um

    d_item.put(d, item);
    item_d.put(item, d);
  }

  // Remove e retorna o item do começo da lista
  // 5*log(n) ~ log(n)
  public Item deleteFront()
  {
    if (d_item.isEmpty()) throw new NoSuchElementException("Chamou o método deleteFront() com uma lista vazia");

    double d = d_item.min();
    Item aux = d_item.get(d);

    d_item.delete(d);
    item_d.delete(aux);

    return aux;
  }

  // Remove e retorna o item do final da lista
  // 5*log(n) ~ log(n)
  public Item deleteBack()
  {
    if (d_item.isEmpty()) throw new NoSuchElementException("Chamou o método deleteBack() com uma lista vazia");

    double d = d_item.max();
    Item aux = d_item.get(d);

    d_item.delete(d);
    item_d.delete(aux);

    return aux;
  }

  // Deleta item da lista
  // 5*log(n) ~ log(n)
  public void delete(Item item)
  {
    if (d_item.isEmpty()) throw new NoSuchElementException("Chamou o método delete() com uma lista vazia");
    if (!item_d.contains(item)) throw new NoSuchElementException("Item '" + item + "' inexistente na lista");

    double d = item_d.get(item);

    d_item.delete(d);
    item_d.delete(item);
  }

  // Adiciona um item na i-ésima posição da lista
  // [ log(n) ou ( log(n) + O(addBack()) ou 5*log(n) ] ~ log(n)
  public void add(int i, Item item)
  {
    if      (i < 0 || i > d_item.size()) throw new IndexOutOfBoundsException("Índice '" + i + "' está fora do alcance da lista");
    else if (i == d_item.size())         addBack(item); // Como a lista vai de 0 até N-1, sendo N o número de elementos da lista, inserir um item na posição de índice N é o mesmo que colocar no final da lista
    else
    {
      double d0 = d_item.select(i - 1);
      double d1 = d_item.select(i);
      double d = (d1 - d0) / 2 + d0;

      d_item.put(d, item);
      item_d.put(item, d);
    }
  }

  // Deleta o i-ésimo item da lista
  // 6*log(n) ~ log(n)
  public Item delete(int i)
  {
    if (d_item.isEmpty()) throw new NoSuchElementException("Chamou o método Item delete() com uma lista vazia");
    if (i < 0 || i >= d_item.size()) throw new IndexOutOfBoundsException("Índice '" + i + "' está fora do alcance da lista");

    double d = d_item.select(i);
    Item aux = d_item.get(d);

    d_item.delete(d);
    item_d.delete(aux);

    return aux;
  }

  // Retorna o i-ésimo elemento da lista sem deletá-lo
  // 4*log(n) ~ log(n)
  public Item get(int i)
  {
    if (d_item.isEmpty()) throw new NoSuchElementException("Chamou o método Item get() com uma lista vazia");
    if (i < 0 || i >= d_item.size()) throw new IndexOutOfBoundsException("Índice fora da lista");

    double d = d_item.select(i);
    return d_item.get(d);
  }

  // Verifica se a lista está vazia
  // O(1)
  public boolean isEmpty()           { return d_item.isEmpty();      }

  // Retorna o número de itens da lista
  // O(1)
  public int size()                  { return d_item.size();         }

  // Verifica se o item está na lista
  // log(n)
  public boolean contains(Item item) { return item_d.contains(item); }

  // Iterador
  // for (double d : list)
  public Iterator<Item> iterator()   { return new ListIterator();    }
  private class ListIterator implements Iterator<Item>
  {
    private int i = 0;
    private int size = d_item.size();
    private double d = d_item.min();
    public boolean hasNext() { return i < size;                           }
    public void remove()     { throw new UnsupportedOperationException(); }
    public Item next()
    {
      if (!hasNext()) throw new NoSuchElementException();

      d = d_item.select(i);
      Item aux = d_item.get(d);
      i++;
      return aux;
    }
  }

  // Unit test
  public static void main(String[] args)
  {
    String[] jojo = new String[7];
    jojo[0] = "AVDOL";
    jojo[1] = "BUCCELLATI";
    jojo[2] = "CAESAR";
    jojo[3] = "DIO";
    jojo[4] = "ENRICO";
    jojo[5] = "FUNNY";
    jojo[6] = "GIOGIO";

/********** Separação de bloco de testes **********/

    StdOut.println("\nBloco 1\nCriação de List vazia, isEmpty()\n");

    List<String> list = new List<String>();
    StdOut.println("isEmpty() = " + list.isEmpty());

/********** Separação de bloco de testes **********/

    StdOut.println("\n********************\n\nBloco 2\naddFront(), addBack(), iterator(), size()\n");

    list.addFront(jojo[3]);
    StdOut.print("addFront(" + jojo[3] + ") - ");
    for (String s : list)
      StdOut.print(s + " " );
    StdOut.println("\nsize - " + list.size() + "\n");

    list.addFront(jojo[2]);
    StdOut.print("addFront(" + jojo[2] + ") - ");
    for (String s : list)
      StdOut.print(s + " " );
    StdOut.println("\nsize - " + list.size() + "\n");

    list.addFront(jojo[1]);
    StdOut.print("addFront(" + jojo[1] + ") - ");
    for (String s : list)
      StdOut.print(s + " " );
    StdOut.println("\nsize - " + list.size() + "\n");

    list.addFront(jojo[0]);
    StdOut.print("addFront(" + jojo[0] + ") - ");
    for (String s : list)
      StdOut.print(s + " " );
    StdOut.println("\nsize - " + list.size() + "\n");

    list.addBack(jojo[4]);
    StdOut.print("addBack(" + jojo[4] + ") - ");
    for (String s : list)
      StdOut.print(s + " " );
    StdOut.println("\nsize - " + list.size() + "\n");

    list.addBack(jojo[5]);
    StdOut.print("addBack(" + jojo[5] + ") - ");
    for (String s : list)
      StdOut.print(s + " " );
    StdOut.println("\nsize - " + list.size() + "\n");

    list.addBack(jojo[6]);
    StdOut.print("addBack(" + jojo[6] + ") - ");
    for (String s : list)
      StdOut.print(s + " " );
    StdOut.println("\nsize - " + list.size());

/********** Separação de bloco de testes **********/

    StdOut.println("\n********************\n\nBloco 3\ndeleteFront(), deleteBack(), void delete(Item), size(), add()\n");

    StdOut.print("deleteFront() - " + list.deleteFront() + "\nResultado - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    StdOut.print("deleteBack() - " + list.deleteBack() + "\nResultado - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    StdOut.print("list.delete('CAESAR')\nResultado - ");
    list.delete("CAESAR");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println();

    StdOut.println("\nsize() - " + list.size() + "\n");

    StdOut.print("add(2, 'CAESAR') - ");
    list.add(2, "CAESAR");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    // Reconstrução da lista
    list.delete("CAESAR");
    list.add(1, "CAESAR");
    list.addFront(jojo[0]);
    list.addBack(jojo[6]);
    StdOut.println("Original:");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println();

/********** Separação de bloco de testes **********/

    StdOut.println("\n********************\n\nBloco 4\nItem delete(int), get(), contains(), isEmpty()\n");

    StdOut.print("list.delete(4) - " + list.delete(4) + "\nResultado - ");
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    StdOut.println("list.get(2) - " + list.get(2));
    StdOut.println("list.get(4) - " + list.get(4) + "\n");

    StdOut.println("list.contains('ENRICO') - " + list.contains(jojo[4]));
    StdOut.println("list.contains('GIOGIO') - " + list.contains("GIOGIO"));
    StdOut.println("list.contains('WRYYYY') - " + list.contains("WRYYYY"));
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");

    StdOut.println("isEmpty() - " + list.isEmpty() + "\n");

    // Retomando a forma original da lista
    StdOut.println("Original:");
    list.add(4, jojo[4]);
    for (String s : list)
      StdOut.print(s + " ");
    StdOut.println("\n");
  }
}
