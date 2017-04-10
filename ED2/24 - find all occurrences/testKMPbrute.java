public class testKMPbrute
{
  public static void main(String[] args)
  {
    In in = new In();
    String txt = in.readAll();
    String pat = "1";

    StdOut.println("KMP:");
    KMP kmp = new KMP(pat);
    for (int i : kmp.findAll(txt))
      StdOut.print(i + " ");
    StdOut.println();

    StdOut.println("\nBrute:");
    for (int i : Brute.findAll(txt, pat))
      StdOut.print(i + " ");
    StdOut.println();
  }
}
