public class isPrime
{
  public static void main(String[] args)
  {
    long P = (long) 4 * 99999 * 99999;
    StdOut.println(P);
    long sqrtNum = (long) Math.sqrt(P);

    for (long i = 2; i <= sqrtNum; i++)
    {
      StdOut.println(P + " % " + i + " = " + (P%i));
      if (P % i == 0)
      {
        i = 2;
        P++;
        sqrtNum = (long) Math.sqrt(P);
      }
    }
    StdOut.println(P + " é primo");
  }
}
