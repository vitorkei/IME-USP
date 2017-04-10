public class mergetest
{
  public static void main(String[]args)
  {
    String[] s = new String[7];
    s[0] = "batatal";
    s[1] = "porto";
    s[2] = "batato";
    s[3] = "porta";
    s[4] = "batata";
    s[5] = "portal";
    s[6] = "portalala";

    Merge.sort(s);

    for (int i = 0; i < 6; i++)
    {
      StdOut.println(s[i]);
      StdOut.println(s[i+1]);
      StdOut.println(s[i].compareTo(s[i+1]) + "\n");
    }
  }
}
