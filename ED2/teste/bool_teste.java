public class bool_teste
{
  public static void main (String[] args)
  {
    boolean[][] teste = new boolean[5][5];
    for (int i = 0; i < 5; i++)
    {
      for (int j = 0; j < 5; j++)
        StdOut.print (teste[i][j] + " ");
      StdOut.println ();
    }
  }
}
