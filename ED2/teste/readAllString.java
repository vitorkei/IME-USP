import java.util.*;

public class readAllString
{
  public static void main(String[] args)
  {
    In in = new In(args[0]);
    while (!in.isEmpty())
    {
      String word = in.readString();
      StdOut.println(word);
    }
    StdOut.println("****************");

    in.close();
    while (!in.isEmpty())
    {
      String word = in.readString();
      StdOut.println(word);
    }
  }
}
