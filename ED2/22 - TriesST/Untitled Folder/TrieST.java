/***************************************************************
* 
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Creative Problem 5.2.8 (Ordered operations for tries)
*
***************************************************************/

public class TrieST<Value> {
/**********************************************************************************/

  boolean prefix;
  boolean isPrefix;

/**********************************************************************************/
    private static final int R = 256;        // extended ASCII

    private Node root;      // root of trie
    private int N;          // number of keys in trie

    // R-way trie node
    private static class Node {
        private Object val;
        private Node[] next = new Node[R];
    }

   /**
     * Initializes an empty string symbol table.
     */
    public TrieST() {
    }


    /**
     * Returns the value associated with the given key.
     * @param key the key
     * @return the value associated with the given key if the key is in the symbol table
     *     and <tt>null</tt> if the key is not in the symbol table
     * @throws NullPointerException if <tt>key</tt> is <tt>null</tt>
     */
    public Value get(String key) {
        Node x = get(root, key, 0);
        if (x == null) return null;
        return (Value) x.val;
    }

    /**
     * Does this symbol table contain the given key?
     * @param key the key
     * @return <tt>true</tt> if this symbol table contains <tt>key</tt> and
     *     <tt>false</tt> otherwise
     * @throws NullPointerException if <tt>key</tt> is <tt>null</tt>
     */
    public boolean contains(String key) {
        return get(key) != null;
    }

    private Node get(Node x, String key, int d) {
        if (x == null) return null;
        if (d == key.length()) return x;
        char c = key.charAt(d);
        return get(x.next[c], key, d+1);
    }

    /**
     * Inserts the key-value pair into the symbol table, overwriting the old value
     * with the new value if the key is already in the symbol table.
     * If the value is <tt>null</tt>, this effectively deletes the key from the symbol table.
     * @param key the key
     * @param val the value
     * @throws NullPointerException if <tt>key</tt> is <tt>null</tt>
     */
    public void put(String key, Value val) {
        if (val == null) delete(key);
        else root = put(root, key, val, 0);
    }

    private Node put(Node x, String key, Value val, int d) {
        if (x == null) x = new Node();
        if (d == key.length()) {
            if (x.val == null) N++;
            x.val = val;
            return x;
        }
        char c = key.charAt(d);
        x.next[c] = put(x.next[c], key, val, d+1);
        return x;
    }

    /**
     * Returns the number of key-value pairs in this symbol table.
     * @return the number of key-value pairs in this symbol table
     */
    public int size() {
        return N;
    }

    /**
     * Is this symbol table empty?
     * @return <tt>true</tt> if this symbol table is empty and <tt>false</tt> otherwise
     */
    public boolean isEmpty() {
        return size() == 0;
    }

    /**
     * Returns all keys in the symbol table as an <tt>Iterable</tt>.
     * To iterate over all of the keys in the symbol table named <tt>st</tt>,
     * use the foreach notation: <tt>for (Key key : st.keys())</tt>.
     * @return all keys in the sybol table as an <tt>Iterable</tt>
     */
    public Iterable<String> keys() {
        return keysWithPrefix("");
    }

    /**
     * Returns all of the keys in the set that start with <tt>prefix</tt>.
     * @param prefix the prefix
     * @return all of the keys in the set that start with <tt>prefix</tt>,
     *     as an iterable
     */
    public Iterable<String> keysWithPrefix(String prefix) {
        Queue<String> results = new Queue<String>();
        Node x = get(root, prefix, 0);
        collect(x, new StringBuilder(prefix), results);
        return results;
    }

    private void collect(Node x, StringBuilder prefix, Queue<String> results) {
        if (x == null) return;
        if (x.val != null) results.enqueue(prefix.toString());
        for (char c = 0; c < R; c++) {
            prefix.append(c);
            collect(x.next[c], prefix, results);
            prefix.deleteCharAt(prefix.length() - 1);
        }
    }

    /**
     * Returns all of the keys in the symbol table that match <tt>pattern</tt>,
     * where . symbol is treated as a wildcard character.
     * @param pattern the pattern
     * @return all of the keys in the symbol table that match <tt>pattern</tt>,
     *     as an iterable, where . is treated as a wildcard character.
     */
    public Iterable<String> keysThatMatch(String pattern) {
        Queue<String> results = new Queue<String>();
        collect(root, new StringBuilder(), pattern, results);
        return results;
    }

    private void collect(Node x, StringBuilder prefix, String pattern, Queue<String> results) {
        if (x == null) return;
        int d = prefix.length();
        if (d == pattern.length() && x.val != null)
            results.enqueue(prefix.toString());
        if (d == pattern.length())
            return;
        char c = pattern.charAt(d);
        if (c == '.') {
            for (char ch = 0; ch < R; ch++) {
                prefix.append(ch);
                collect(x.next[ch], prefix, pattern, results);
                prefix.deleteCharAt(prefix.length() - 1);
            }
        }
        else {
            prefix.append(c);
            collect(x.next[c], prefix, pattern, results);
            prefix.deleteCharAt(prefix.length() - 1);
        }
    }

    /**
     * Returns the string in the symbol table that is the longest prefix of <tt>query</tt>,
     * or <tt>null</tt>, if no such string.
     * @param query the query string
     * @return the string in the symbol table that is the longest prefix of <tt>query</tt>,
     *     or <tt>null</tt> if no such string
     * @throws NullPointerException if <tt>query</tt> is <tt>null</tt>
     */
    public String longestPrefixOf(String query) {
        int length = longestPrefixOf(root, query, 0, -1);
        if (length == -1) return null;
        else return query.substring(0, length);
    }

    // returns the length of the longest string key in the subtrie
    // rooted at x that is a prefix of the query string,
    // assuming the first d character match and we have already
    // found a prefix match of given length (-1 if no such match)
    private int longestPrefixOf(Node x, String query, int d, int length) {
        if (x == null) return length;
        if (x.val != null) length = d;
        if (d == query.length()) return length;
        char c = query.charAt(d);
        return longestPrefixOf(x.next[c], query, d+1, length);
    }

    /**
     * Removes the key from the set if the key is present.
     * @param key the key
     * @throws NullPointerException if <tt>key</tt> is <tt>null</tt>
     */
    public void delete(String key) {
        root = delete(root, key, 0);
    }

    private Node delete(Node x, String key, int d) {
        if (x == null) return null;
        if (d == key.length()) {
            if (x.val != null) N--;
            x.val = null;
        }
        else {
            char c = key.charAt(d);
            x.next[c] = delete(x.next[c], key, d+1);
        }

        // remove subtrie rooted at x if it is completely empty
        if (x.val != null) return x;
        for (int c = 0; c < R; c++)
            if (x.next[c] != null)
                return x;
        return null;
    }

/**********************************************************************************/

  public String floor(String x)
  {
    if (x == null) throw new NullPointerException("called floor() with null String argument");
    if (contains(x)) return x;

    Stack<String> word = new Stack<String>();
    prefix = true;
    isPrefix = false;

    floor(root, x, 0, word);
    if (word.isEmpty()) return null;
    int size = word.size();
    char[] aux = new char[size];
    String s;
    if (!isPrefix)
    {
      for (int i = size - 1; i > -1; i--)
      {
        String t = word.pop();
        char c = t.charAt(0);
        aux[i] = c;
      }
      s = new String(aux);
    }
    else
      s = word.pop();

    return s;
  }
  // Considera que a palavra não existe na tabela de símbolos
  private void floor(Node x, String key, int d, Stack<String> word)
  {
    if (prefix)
    {
      if (d < key.length())
      {
        char c = key.charAt(d);
        if (x.next[c] == null)
        {
          prefix = false;

          // Recebe o caractere imediatamente menor e existente
          int i = c;
          while (i > -1 && x.next[i] == null) i--;

          if (i != -1)
          {
            c = (char) i;
  //          StdOut.println("null c = " + c);
            char[] aux = new char[1];
            aux[0] = c;
            String s = new String(aux);
            word.push(s);
  //          StdOut.println("word = " + word.toString());

            floor(x.next[c], key, d+1, word);
          }
          else
          {
            word.pop();
            d--;
            c = key.charAt(d);
            i = c - 1;
            while (x.next[i] == null)
            {
              i--;
              StdOut.println("d = " + d);
              StdOut.println((char)i);
              if (i == -1)
              {
                d--;
//            StdOut.println("d = " + d);
                if (d == -1) return;
                c = key.charAt(d);
            StdOut.println("c' = " + c);
                c--;
                i = c;
              }
            }

            c = (char) i;
            char[] aux = new char[1];
            aux[0] = c;
            String s = new String(aux);
            word.push(s);

            floor(x.next[c], key, d+1, word);
          }
        }
        else
        {
  //        StdOut.println("prefix c = " + c);
          char[] aux = new char[1];
          aux[0] = c;
          String s = new String(aux);
          word.push(s);
  //        StdOut.println("word = " + word.toString());
          floor(x.next[c], key, d+1, word);
        }
      }
      else
      {
        String s = longestPrefixOf(key);
        while(!word.isEmpty())
          word.pop();
        word.push(s);
        isPrefix = true;
      }
    }
    else
    {
      int i = 255;
      while (i > -1 && x.next[i] == null)i--;
      char c = (char) i;

      if (i != -1)
      {
        char[] aux = new char[1];
        aux[0] = c;
        String s = new String(aux);
        word.push(s);
//        StdOut.println("word = " + word.toString());

        floor(x.next[c], key, d+1, word);
      }
    }
//    StdOut.println("prefix = " + prefix);
  }
/*
  public String ceil(String x)
  {
    if (x == null) throw new NullPointerException("called ceil() with null String argument");
    if (contains(x)) return x;
  }
  // Considera que a palavra não existe na tabela de símbolos
  private void ceiling()
  {

  }

  public String select(int i)
  {

  }

  public int rank(String x)
  {

  }

/**********************************************************************************/

    /**
     * Unit tests the <tt>TrieST</tt> data type.
     */
    public static void main(String[] args) {

        // build symbol table from standard input
        TrieST<Integer> st = new TrieST<Integer>();
        for (int i = 0; !StdIn.isEmpty(); i++) {
            String key = StdIn.readString();
            st.put(key, i);
        }

        // print results
        if (st.size() < 100) {
            StdOut.println("keys(\"\"):");
            for (String key : st.keys()) {
                StdOut.println(key + " " + st.get(key));
            }
            StdOut.println();
        }
/*
        StdOut.println("longestPrefixOf(\"shell\"):");
        StdOut.println(st.longestPrefixOf("shell"));
        StdOut.println();

        StdOut.println("longestPrefixOf(\"quicksort\"):");
        StdOut.println(st.longestPrefixOf("quicksort"));
        StdOut.println();

        StdOut.println("keysWithPrefix(\"shor\"):");
        for (String s : st.keysWithPrefix("shor"))
            StdOut.println(s);
        StdOut.println();

        StdOut.println("keysThatMatch(\".he.l.\"):");
        for (String s : st.keysThatMatch(".he.l."))
            StdOut.println(s);
*/
      StdOut.println();
      StdOut.println("floor(shit)  = " + st.floor("shit"));
      StdOut.println("floor(she)   = " + st.floor("she"));
      StdOut.println("floor(sgh)   = " + st.floor("sgh"));
      StdOut.println("floor(shell) = " + st.floor("shell"));
      StdOut.println("floor(shawn) = " + st.floor("shawn"));
      StdOut.println("floor(bar)   = " + st.floor("bar"));
      StdOut.println("floor(tho)   = " + st.floor("tho"));
      StdOut.println("floor(saw)   = " + st.floor("saw"));
//      StdOut.println(st.floor("shawn"));
//      StdOut.println(st.floor("shell"));
    }
}

