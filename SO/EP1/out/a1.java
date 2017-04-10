public class a1
{
  public static void main(String[] args)
  {
    double tf, tr;
    double media_tf = 0, media_tr = 0;
    while (!StdIn.isEmpty())
    {
      tf = StdIn.readDouble();
      tr = StdIn.readDouble();
      media_tf += tf;
      media_tr += tr;
    }
    media_tf /= 30;
    media_tr /= 30;
    StdOut.println(media_tf + " " + media_tr);
  }
}
