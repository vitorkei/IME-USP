/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vitor Kei Taira Tamada
* NUSP: 8516250
* Exercise 3.4.18 (Algs4)
* 
***************************************************************/

/**
 *  The <tt>SeparateChainingHashST</tt> class represents a symbol table of generic
 *  key-value pairs.
 *  It supports the usual <em>put</em>, <em>get</em>, <em>contains</em>,
 *  <em>delete</em>, <em>size</em>, and <em>is-empty</em> methods.
 *  It also provides a <em>keys</em> method for iterating over all of the keys.
 *  A symbol table implements the <em>associative array</em> abstraction:
 *  when associating a value with a key that is already in the symbol table,
 *  the convention is to replace the old value with the new value.
 *  Unlike {@link java.util.Map}, this class uses the convention that
 *  values cannot be <tt>null</tt>&mdash;setting the
 *  value associated with a key to <tt>null</tt> is equivalent to deleting the key
 *  from the symbol table.
 *  <p>
 *  This implementation uses a separate chaining hash table. It requires that
 *  the key type overrides the <tt>equals()</tt> and <tt>hashCode()</tt> methods.
 *  The expected time per <em>put</em>, <em>contains</em>, or <em>remove</em>
 *  operation is constant, subject to the uniform hashing assumption.
 *  The <em>size</em>, and <em>is-empty</em> operations take constant time.
 *  Construction takes constant time.
 *  <p>
 *  For additional documentation, see <a href="http://algs4.cs.princeton.edu/34hash">Section 3.4</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *  For other implementations, see {@link ST}, {@link BinarySearchST},
 *  {@link SequentialSearchST}, {@link BST}, {@link RedBlackBST}, and
 *  {@link LinearProbingHashST},
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 */
public class SeparateChainingHashST<Key, Value> {
    private static final int INIT_CAPACITY = 4;

    // largest prime <= 2^i for i = 3 to 31
    // not currently used for doubling and shrinking
    private static final int[] primes = {
       7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191, 16381,
       32749, 65521, 131071, 262139, 524287, 1048573, 2097143, 4194301,
       8388593, 16777213, 33554393, 67108859, 134217689, 268435399,
       536870909, 1073741789, 2147483647
    };

    private int N;                                // number of key-value pairs
    private int M;                                // hash table size
    private SequentialSearchST<Key, Value>[] st;  // array of linked-list symbol tables

    private int logM;
    private int K;    // Limite do tamanho das listas ligadas da tabela

    /**
     * Initializes an empty symbol table.
     */
    public SeparateChainingHashST() {
        this(INIT_CAPACITY);
    } 

    /**
     * Initializes an empty symbol table with <tt>M</tt> chains.
     * @param M the initial number of chains
     */
    public SeparateChainingHashST(int M) {
        this.M = M;
        st = (SequentialSearchST<Key, Value>[]) new SequentialSearchST[M];
        for (int i = 0; i < M; i++)
            st[i] = new SequentialSearchST<Key, Value>();
    } 

/******************************************************************************************/
// Construtor e método escritos para este exercício-programa
// Alguns outros métodos também foram levemente alterados (resize(), hash(), put())

  // Inicializa uma tabela de símbolos vazia com M Chains.
  // Os Chains da tabela tem tamanho máximo K
  public SeparateChainingHashST(int M, int K)
  {
    this.M = M;
    this.logM = (int) (Math.log(M) / Math.log(2));
    this.K = K;
    st = (SequentialSearchST<Key, Value>[]) new SequentialSearchST[K];
    for (int i = 0; i < K; i++)
      st[i] = new SequentialSearchST<Key, Value>();
  }

  public int[] getSizes()
  {
    int[] sizes = new int[M];
    for (int i = 0; i < M; i++)
      sizes[i] = st[i].size();

    return sizes;
  }

/******************************************************************************************/

    // resize the hash table to have the given number of chains b rehashing all of the keys
    private void resize(int chains) {
        SeparateChainingHashST<Key, Value> temp;
        if (K == 0)
          temp = new SeparateChainingHashST<Key, Value>(chains);
        else
          temp = new SeparateChainingHashST<Key, Value>(chains, K);

        for (int i = 0; i < M; i++) {
            for (Key key : st[i].keys()) {
                temp.put(key, st[i].get(key));
            }
        }
        this.M  = temp.M;
        this.N  = temp.N;
        this.st = temp.st;

        // Adicionado para o exercício programa
        this.logM = (int) (Math.log (temp.M) / Math.log(2));
    }

    // hash value between 0 and M-1
    private int hash(Key key) {
      // Se um dos dois primeiros construtores for utilizado, o K é definido como zero.
      // Por outro lado, se o terceiro construtor for utilizado, K não pode ser zero
      if (K == 0)
        return (key.hashCode() & 0x7fffffff) % M;
      else
      {
        int t = key.hashCode() & 0x7fffffff;
        if (logM < 26)
          t = t % primes[logM + 4]; // 4 é um valor arbitrário
        return t % M;
      }
    } 

    /**
     * Returns the number of key-value pairs in this symbol table.
     *
     * @return the number of key-value pairs in this symbol table
     */
    public int size() {
        return N;
    } 

    /**
     * Returns true if this symbol table is empty.
     *
     * @return <tt>true</tt> if this symbol table is empty;
     *         <tt>false</tt> otherwise
     */
    public boolean isEmpty() {
        return size() == 0;
    }

    /**
     * Returns true if this symbol table contains the specified key.
     *
     * @param  key the key
     * @return <tt>true</tt> if this symbol table contains <tt>key</tt>;
     *         <tt>false</tt> otherwise
     * @throws NullPointerException if <tt>key</tt> is <tt>null</tt>
     */
    public boolean contains(Key key) {
        if (key == null) throw new NullPointerException("argument to contains() is null");
        return get(key) != null;
    } 

    /**
     * Returns the value associated with the specified key in this symbol table.
     *
     * @param  key the key
     * @return the value associated with <tt>key</tt> in the symbol table;
     *         <tt>null</tt> if no such value
     * @throws NullPointerException if <tt>key</tt> is <tt>null</tt>
     */
    public Value get(Key key) {
        if (key == null) throw new NullPointerException("argument to get() is null");
        int i = hash(key);
        return st[i].get(key);
    } 

    /**
     * Inserts the specified key-value pair into the symbol table, overwriting the old 
     * value with the new value if the symbol table already contains the specified key.
     * Deletes the specified key (and its associated value) from this symbol table
     * if the specified value is <tt>null</tt>.
     *
     * @param  key the key
     * @param  val the value
     * @throws NullPointerException if <tt>key</tt> is <tt>null</tt>
     */
    public void put(Key key, Value val) {
        if (key == null) throw new NullPointerException("first argument to put() is null");
        if (val == null) {
            delete(key);
            return;
        }

        // Se um dos dois primeiros construtores for utilizado, o K é definido como zero.
        // Por outro lado, se o terceiro construtor for utilizado, K não pode ser zero
        if (K == 0)
        {
          // double table size if average length of list >= 10
          if (N >= 10*M) resize(2*M);
        }
        else
        {
          int[] temp_sizes = new int[M];
          temp_sizes = getSizes();
          int media = 0;
          for (int j = 0; j < M; j++)
            media += temp_sizes[j];
          media /= M;
          if (media > K)
            resize (2*M);
        }

        int i = hash(key);
        if (!st[i].contains(key)) N++;
        st[i].put(key, val);
    } 

    /**
     * Removes the specified key and its associated value from this symbol table     
     * (if the key is in this symbol table).    
     *
     * @param  key the key
     * @throws NullPointerException if <tt>key</tt> is <tt>null</tt>
     */
    public void delete(Key key) {
        if (key == null) throw new NullPointerException("argument to delete() is null");

        int i = hash(key);
        if (st[i].contains(key)) N--;
        st[i].delete(key);

        if (K == 0)
          // halve table size if average length of list <= 2
          if (M > INIT_CAPACITY && N <= 2*M) resize(M/2, 0);
    } 

    // return keys in symbol table as an Iterable
    public Iterable<Key> keys() {
        Queue<Key> queue = new Queue<Key>();
        for (int i = 0; i < M; i++) {
            for (Key key : st[i].keys())
                queue.enqueue(key);
        }
        return queue;
    } 


    /**
     * Unit tests the <tt>SeparateChainingHashST</tt> data type.
     */
    public static void main(String[] args) { 
        SeparateChainingHashST<String, Integer> st = new SeparateChainingHashST<String, Integer>(1, 2);

        for (int i = 0; !StdIn.isEmpty(); i++) {
            String key = StdIn.readString();
            st.put(key, i);
        }

        // print keys
        for (String s : st.keys()) 
            StdOut.println(s + " " + st.get(s)); 

    }

}

