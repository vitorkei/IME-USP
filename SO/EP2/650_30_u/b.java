/*
  Recebe um arquivo com o
	Elapsed (wall clock) time (h:mm:ss or m:ss)
  Maximum resident set size (kbytes)
  e faz as primeiras 30 linhas terem o System time e as 30 linhas seguintes terem o Maximum resident set size
*/

public class b
{
  public static void main(String[] args)
  {
    String[] s = new String[60];
    for(int i = 0; i < 60; i++)
      s[i] = StdIn.readLine();
    for(int i = 0; i < 60; i += 2)
      StdOut.println(s[i]);
    for(int i = 1; i < 60; i += 2)
      StdOut.println(s[i]);
  }
}
