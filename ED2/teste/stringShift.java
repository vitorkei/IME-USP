import java.util.*;

public class stringShift
{
  public static String shift (String s)
  {
    return s.charAt(s.length() - 1) + s.substring(0, s.length() - 1);
  }

  public static void main(String[] args)
  {
    String s = "batata";
    for (int i = 0; i < s.length(); i++)
    {
      s = shift(s);
      StdOut.println(s);
    }
  }
}
