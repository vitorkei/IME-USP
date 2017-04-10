public class dp
{
  public static void main(String[] args)
  {
    double sigma = 0, x, y;
    while (!StdIn.isEmpty())
    {
      x = StdIn.readDouble();
      y = StdIn.readDouble();
      sigma += (x - 4.7333) * (x - 4.7333);
    }
    sigma /= 29;
    StdOut.println(sigma);
    sigma = Math.sqrt(sigma);
    StdOut.println(sigma);
  }
}
