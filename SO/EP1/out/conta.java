/* Tira a média de tr e de tf de um output de 500 processos */

public class conta
{
  public static void main(String[] args)
  {
    String s;
    double tf, tr, count_tf = 0, count_tr = 0;
    while (!StdIn.isEmpty())
    {
      s = StdIn.readString();
      if (s.equals("0")) break;
      tf = StdIn.readDouble();
      tr = StdIn.readDouble();
      count_tr += tr;
      count_tf += tf;
    }
    count_tr /= 500;
    count_tf /= 500;
    StdOut.println(count_tr);
    StdOut.println(count_tf + "\n");
  }
}
