public class breakTeste
{
  public static void main(String[] args)
  {
    for (int i = 0; i < 100; i++)
    {
      StdOut.println("i = " + i);
      for (int j = -100; j < 0; j++)
      {
        StdOut.println("j = " + j);
        if (j == -42) break;
      }
    }
  }
}
