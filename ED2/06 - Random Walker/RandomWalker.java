/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Creative Exercise 3.2.47 (Random walker; IntroCS)
*
***************************************************************/

public class RandomWalker
{
  int x, y;

  public RandomWalker (int x0, int y0)
  {
    x = x0;
    y = y0;
  }

  // O plano imaginado é o cartesiano, com o Walker podendo percorrer qualquer um dos 4 quadrantes
  public void step ()
  {
    double dir = StdRandom.uniform ();  // dir == direction

    // Vai para a esquerda
    if (dir < .25)
      x--;

    // Vai para cima
    else if (dir >= .25 && dir < .5)
      y++;

    // Vai para a direita
    else if (dir >= .5 && dir < .75)
      x++;

    // Vai para baixo
    else if (dir >= .75)
      y--;

  }

  // A distância é sempre um número positivo
  public double distance ()
  {
    return Math.hypot (x, y);
  }

  // Teste
  public static void main (String[] args)
  {
    int N = Integer.parseInt (args[0]);
    RandomWalker rw = new RandomWalker (0, 0);

    for (int i = 0; i < N; i++)
      rw.step ();

    // Imprime as coordenadas da célula onde o Walker parou e a distância dessa célula até a origem
    StdOut.println ("(x, y) = (" + rw.x + ", " + rw.y + ")");
    StdOut.println (rw.distance ());
  }
}
