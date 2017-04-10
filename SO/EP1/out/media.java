/* Tira a média de tr e de tf da média de cada um dos 30 outputs */

public class media
{
  public static void main(String[] args)
  {
    double tr, tf, media_tr = 0, media_tf = 0;
    while(!StdIn.isEmpty())
    {
      tr = StdIn.readDouble();
      tf = StdIn.readDouble();
      media_tr += tr;
      media_tf += tf;
    }
    media_tr /= 30;
    media_tf /= 30;
    StdOut.println(media_tr);
    StdOut.println(media_tf + "\n");
  }
}
