/*
  Retira os textos System time e Maximum resident set size, deixando apenas os números
*/

public class c
{
  public static void main(String[] args)
  {
    for(int i = 0; i < 30; i++)
    {
      String s = StdIn.readLine();
      for(int j = 48; j < s.length(); j++)
        StdOut.print(s.charAt(j));
      StdOut.println();
    }
    for(int i = 0; i < 30; i++)
    {
      String s = StdIn.readLine();
      for(int j = 37; j < s.length(); j++)
        StdOut.print(s.charAt(j));
      StdOut.println();
    }
  }
}

