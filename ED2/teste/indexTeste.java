public class indexTeste
{
  public static void main(String[] args)
  {
    IndexSET<String> words = new IndexSET<String>();
    IndexSET<String> aux   = new IndexSET<String>();
    while (!StdIn.isEmpty())
    {
      String s = StdIn.readString();
      words.add(s);
      aux.add(s);
    }
    for (String s : words.keys())
      StdOut.println(s);
    words = new IndexSET<String>();
    for (String s : words.keys())
      StdOut.println(s);
  }
}
