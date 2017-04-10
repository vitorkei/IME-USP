public class stringtest
{
  public static void main(String[] args)
  {
    String a = "brown";
    String b = "brow";
    int cmp = b.compareTo(a);
    StdOut.println(cmp);

    char[] j = new char[1];
    j[0] = (char) (23 % 10) + 48;
    String s = new String(j);
    StdOut.println("\ns = " + s);

    StdOut.println("**********");
    String c = "oi";
    if (c.charAt(0) == 'o')
      StdOut.println(c.charAt(0));
  }
}
