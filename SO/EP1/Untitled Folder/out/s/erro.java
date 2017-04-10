public class erro
{
    public static void main(String[] args)
    {
        double erro, total = 0;
        while(!StdIn.isEmpty())
        {
            erro = StdIn.readDouble();
            total += erro;
        }
        total /= 30;
        StdOut.println(total);
    }
}
