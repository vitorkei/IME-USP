import java.util.Iterator;
import java.util.NoSuchElementException;

public class List<Item extends Comparable<Item>> implements Iterable<Item>
{
  private ST<Double, Item>    d_item;   // Busca pelo índice
  private ST<Item, Double>    item_d;   // Busca pelo ítem
  private ST<Double, Integer> ref;      // ST auxiliar

  // Inicializa três tabelas de símbolos vazias
  public List()
  {
    d_item = new ST<Double, Item>();
    item_d = new ST<Item, Double>();
    ref = new ST<Integer, Double>();
  }

  // Adiciona um item ao começo da lista
  public void addFront(Item item)
  {
    double d;
    int i;
    // Caso de a tabela ainda estar vazia
    if (d_item.isEmpty())
    {
      d = 0.1;
      i = 0;
    }
    // (Chave de menor valor - 0) / 2, garantindo que será maior que zero, mas ainda sim será a menor chave
    else
    {
      d = d_item.min() / 2;
      i = ref.min() - 1;
    }

    d_item.put(d, item);
    item_d.put(item, d);
    ref.put(d, i);
  }

  // Adiciona um item ao final da lista
  public void addBack(Item item)
  {
    double d;
    int i;
    if (d_item.isEmpty())
    {
      d = 0.9;
      i = 0;
    }
    // (1 - Chave de maior valor) / 2, garantindo que será menor do que 1, mas ainda sim a maior chave
    else
    {
      d = (1 - d_item.max()) / 2;
      i = ref.max() + 1;
    }

    d_item.put(d, item);
    item_d.put(item, d);
    ref.put(d, i);
  }

  // Remove e retorna o item do começo da lista
  public Item deleteFront()
  {
    if (d_item.isEmpty()) throw new NoSuchElementException("Chamou o método deleteFront() com uma lista vazia");

    double d = d_item.min();
    Item aux = d_item.get(d);

    d_item.delete(d);
    item_d.delete(aux);
    ref.delete(d);

    return aux;
  }

  // Remove e retorna o item do final da lista
  public Item deleteBack()
  {
    if (d_item.isEmpty()) throw new NoSuchElementException("Chamou o método deleteFront() com uma lista vazia");

  double d = d_item.max();
  Item aux = d_item.get(d);

  d_item.delete(d);
  item_d.delete(aux);
  ref.delete(d);

  return aux;
  }

  // Deleta item da lista
  public void delete(Item item)
  {
    if (d_item.isEmpty()) throw new NoSuchElementException("Chamou o método deleteFront() com uma lista vazia");

    double d = item_d.get(item);

    d_item.delete(d);
    item_d.delete(item);
    ref.delete(d);
  }

  // Adiciona item na i-ésima posição da lista
  public void add(int i, Item item)
  {
    
  }

  // Deleta e retorna o i-ésimo item da lista
  public Item delete(int i)
  {
    if (d_item.isEmpty()) throw new NoSuchElementException("Chamou o método deleteFront() com uma lista vazia");
  }

  // Retorna o i-ésimo elemento da lista
  public Item get(int i)
  {
    double d;
    for (d : ref.keys())
      if (ref.get(d) == i)
        break;
    return d_item.get(d);
  }
}
