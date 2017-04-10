/*
  Lê e imprime apenas as linhas de tempo e maior uso da memória
  System time (seconds)
  Maximum resident set size (kbytes)
*/

public class a
{
  public static void main(String[] args)
  {
    String s;
    for(int i = 0; i < 23; i++)
    {
      s = StdIn.readLine();
      if(i == 4 || i == 9)
        StdOut.println(s);
    }
  }
}
