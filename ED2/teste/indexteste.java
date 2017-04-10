public class indexteste
{
  public static void main(String[] args)
  {
    IndexSET<String> index = new IndexSET<String>();
    while (!StdIn.isEmpty())
    {
      String s = StdIn.readString();
      index.add(s);
    }
    for (String s : index.keys())
      StdOut.println(s);
  }
}
