public class dConextoTeste
{
  public static void main (String[] args)
  {
    int N = Integer.parseInt (args[0]);
    StdOut.println (N + " 0.01");
//    StdOut.println (.5 + " " + 1.0);

    for (int i = 0; i < N; i++)
    {
      double x = StdRandom.uniform ();
//      double y = StdRandom.uniform ();
      double y = StdRandom.uniform (0, .5);
      StdOut.println (x + " " + y);
    }
  }
}
