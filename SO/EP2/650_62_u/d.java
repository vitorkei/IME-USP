/*
  Tira a média e variância do tempo e, em seguida, média e variância do espaço usado
*/

public class d
{
  public static void main(String[] args)
  {
    /* Tempo */
    double media_t = 0, total_t = 0, var_t = 0;
    double[] t = new double[30];
    for(int i = 0; i < 30; i++)
    {
      t[i] = StdIn.readDouble();
      total_t = total_t + t[i];
    }
    
    media_t = total_t / 30;
    StdOut.println("total_t = " + total_t);
    StdOut.println("media_t = " + media_t);
    
    for(int i = 0; i < 30; i++)
    {
      double aux = t[i] - media_t;
      var_t += (aux * aux);
    }
    var_t /= 29;
    StdOut.println("var_t = " + var_t);
    StdOut.println();
    
    /* Espaço */
    double media_m = 0, total_m = 0, var_m = 0;
    double[] m = new double[30];
    for(int i = 0; i < 30; i++)
    {
      m[i] = StdIn.readDouble();
      total_m = total_m + m[i];
    }
    
    media_m = total_m / 30;
    StdOut.println("total_m = " + total_m);
    StdOut.println("media_m = " + media_m);
    
    for(int i = 0; i < 30; i++)
    {
      double aux = m[i] - media_m;
      var_m += (aux * aux);
    }
    var_m /= 29;
    StdOut.println("var_m = " + var_m);
    StdOut.println();
  }
}
