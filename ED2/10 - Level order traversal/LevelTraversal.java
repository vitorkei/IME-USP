/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Creative Problem 3.2.37 (Level-order traversal; Algs4)
*
***************************************************************/

import java.util.NoSuchElementException;

public class LevelTraversal<Key extends Comparable<Key>, Value>
{
  // Raíz da BST
  private Node root;

  private class Node
  {
    private Key key;          // Chave
    private Value val;        // Valor associado à chave
    private Node left, right; // Subárvores esquerda e direita
    private int N;            // Número de nós na subárvore

    public Node (Key key, Value val, int N)
    {
      this.key = key;
      this.val = val;
      this.N = N;
    }
  }

  // Inicializa uma tabela de símbolos vazia
  public LevelTraversal ()  {}

  // Retorna true se a tabela de símbolos estiver vazia
  public boolean isEmpty ()  { return size() == 0; }

  // Retorna o número de pares chave-valor na tabela de símbolos
  public int size ()  { return size (root); }

  // Retorna o número de pares chave-valor enraizados em x
  private int size (Node x)
  {
    if (x == null) return 0;
    else return x.N;
  }

  // Verifica se a tabela contém a chave dada
  public boolean contains (Key key)
  {
    if (key == null) throw new NullPointerException ("Argumento para o método contains() é null");
    return get (key) != null;
  }

  // Retorna o valor associado à chave dada
  public Value get (Key key)  { return get (root, key); }
  private Value get (Node x, Key key)
  {
    if (x == null) return null;
    int cmp = key.compareTo (x.key);
    if      (cmp < 0) return get (x.left, key);
    else if (cmp > 0) return get (x.right, key);
    else              return x.val;
  }

  // Insere um par chave-valor na tabela de símbolos.
  // Se a chave dada já estiver na tabela, sobrescreve seu valor
  // Se o valor do par inserido for null, apaga a chave e seu respectivo valor da tabela
  public void put (Key key, Value val)
  {
    if (key == null) throw new NullPointerException ("Argumento key é null");
    if (val == null)
    {
      delete (key);
      return;
    }
    root = put (root, key, val);
    assert check ();
  }
  private Node put (Node x, Key key, Value val)
  {
    if (x == null) return new Node (key, val, 1);
    int cmp = key.compareTo (x.key);
    if      (cmp < 0) x.left = put (x.left, key, val);
    else if (cmp > 0) x.right = put (x.right, key, val);
    else              x.val = val;
    x.N = 1 + size (x.left) + size (x.right);
    return x;
  }

  // Remove o par chave-valor cuja chave é a menor da tabela de símbolos
  public void deleteMin ()
  {
    if (isEmpty ()) throw new NoSuchElementException ("Underflow da tabela de símbolos");
    root = deleteMin (root);
    assert check ();
  }
  private Node deleteMin (Node x)
  {
    if (x.left == null) return x.right;
    x.left = deleteMin (x.left);
    x.N = size (x.left) + size (x.right) + 1;
    return x;
  }

  // Remove um par chave-valor da tabela de símbolos
  public void delete (Key key)
  {
    if (key == null) throw new NullPointerException ("Argumento do método delete() é null");
    root = delete (root, key);
    assert check ();
  }
  private Node delete (Node x, Key key)
  {
    if (x == null) return null;

    int cpm = key.compareTo (x.key);
    if      (cpm < 0) x.left = delete (x.left, key);
    else if (cpm > 0) x.right = delete (x.right, key);
    else
    {
      if (x.right == null) return x.left;
      if (x.left  == null) return x.right;
      Node t = x;
      x.right = deleteMin (t.right);
      x.left = t.left;
    }
    x.N = size (x.left) + size (x.right) + 1;
    return x;
  }

  // Retorna a menor chave da tabela de símbolos
  public Key min ()
  {
    if (isEmpty ()) throw new NoSuchElementException ("Chamou o método min() com uma tabela de símbolos vazia");
    return min (root).key;
  }
  private Node min (Node x)
  {
    if (x.left == null) return x;
    else                return min (x.left);
  }

  // Retorna a maior chave da tabela de símbolos
  public Key max ()
  {
    if (isEmpty ()) throw new NoSuchElementException ("Chamou o método max() com uma tabela de símbolos vazia");
    return max (root).key;
  }
  private Node max (Node x)
  {
    if (x.right == null) return x;
    else                 return max (x.right);
  }

  // Retorna a k-ésima menor chave da tabela de símbolos
  public Key select (int k)
  {
    if (k < 0 || k >= size ()) throw new IllegalArgumentException ();
    Node x = select (root, k);
    return x.key;
  }
  private Node select (Node x, int k)
  {
    if (x == null) return null;
    int t = size (x.left);
    if (t > k)      return select (x.left, k);
    else if (t < k) return select (x.right, k - t - 1);
    else            return x;
  }

  // Retorna o número de chaves que são estritamente menores que a chave dada na tabela de símbolos
  public int rank (Key key)
  {
    if (key == null) throw new NullPointerException ("Argumento do método rank() é null");
    return rank (key, root);
  }
  // Número de chaves na subárvore abaixo da chave dada
  private int rank (Key key, Node x)
  {
    if (x == null) return 0;
    int cmp = key.compareTo (x.key);
    if (cmp < 0)      return rank (key, x.left);
    else if (cmp > 0) return 1 + size (x.left) + rank (key, x.right);
    else              return size (x.left);
  }

  // Retorna todas as chaves da tabela de símbolos como um Iterable
  public Iterable<Key> keys ()  { return keys (min (), max ()); }
  // Retorna todas as chaves da tabela de símbolos no alcance dado como um Iterable
  public Iterable<Key> keys (Key lo, Key hi)
  {
    if (lo == null) throw new NullPointerException ("Primeiro argumento do método keys () é null");
    if (hi == null) throw new NullPointerException ("Segundo argumento do método keys () é null");

    Queue<Key> queue = new Queue<Key> ();
    keys (root, queue, lo, hi);
    return queue;
  }
  private void keys (Node x, Queue<Key> queue, Key lo, Key hi)
  {
    if (x == null) return;
    int cpmlo = lo.compareTo (x.key);
    int cpmhi = hi.compareTo (x.key);
    if (cpmlo < 0) keys (x.left, queue, lo, hi);
    if (cpmlo <= 0 && cpmhi >= 0) queue.enqueue (x.key);
    if (cpmhi > 0) keys (x.right, queue, lo, hi);
  }

  // Imprime as chaves da árvore segundo o enunciado
  public void printLevel (Key key)
  {
    if (key == null) throw new NullPointerException ("Argumento de printLevel() é null");
    Node x = root;

    // Verifica se o nó que está sendo visto possui a chave buscada
    int cmp = key.compareTo (x.key);
    while (cmp != 0)
    {
      cmp = key.compareTo (x.key);
      if (cmp < 0) x = x.left;
      if (cmp > 0) x = x.right;
    }
    for (Key i : printLevel (x))
      StdOut.print (i + " ");
  }
  private Iterable<Key> printLevel (Node subroot)
  {
    Queue<Key> keys = new Queue<Key> ();
    Queue<Node> queue = new Queue<Node> ();
    queue.enqueue (subroot);
    while (!queue.isEmpty ())
    {
      Node x = queue.dequeue ();
      if (x == null) continue;
      keys.enqueue (x.key);
      queue.enqueue (x.left);
      queue.enqueue (x.right);
    }
    return keys;
  }

  /* Verifica a integridade da estrutura de dados */

  private boolean check ()
  {
    if (!isBST ())            StdOut.println ("Não está em ordem simétrica");
    if (!isSizeConsistent ()) StdOut.println ("Contador de subárvore inconsistente");
    if (!isRankConsistent ()) StdOut.println ("Ranks inconsistentes");
    return isBST () && isSizeConsistent () && isRankConsistent ();
  }

  private boolean isBST ()  { return isBST (root, null, null); }
  private boolean isBST (Node x, Key min, Key max)
  {
    if (x == null) return true;
    if (min != null && x.key.compareTo(min) <= 0) return false;
    if (max != null && x.key.compareTo(max) >= 0) return false;
    return isBST (x.left, min, x.key) && isBST (x.right, x.key, max);
  }

  private boolean isSizeConsistent ()  { return isSizeConsistent (root); }
  private boolean isSizeConsistent (Node x)
  {
    if (x == null) return true;
    if (x.N != size (x.left) + size (x.right) + 1) return false;
    return isSizeConsistent (x.left) && isSizeConsistent (x.right);
  }

  private boolean isRankConsistent ()
  {
    for (int i = 0; i < size (); i++)
      if (i != rank (select (i))) return false;
    for (Key key : keys ())
      if (key.compareTo (select (rank (key))) != 0) return false;
    return true;
  }

  public static void main (String[] args)
  {
    int N = StdIn.readInt ();
    int M = StdIn.readInt ();
    LevelTraversal<Integer, Integer> lt = new LevelTraversal<Integer, Integer> ();

    // Constrói a BST. O valor não é utilizado neste programa, então ele é igual a sua chave apenas como um tipo de placeholder
    for (int i = 0; i < N; i++)
    {
      int key = StdIn.readInt ();
      lt.put (key, key);
    }

    // Executa o método printLevel(), imprimindo as chaves de cada nó da BST segundo
    // a forma exigida no enunciado
    for (int i = 0; i < M; i++)
    {
      int key = StdIn.readInt ();
      lt.printLevel (key);
      StdOut.println ();
    }
  }
}
