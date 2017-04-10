/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Web Exercise 1.5.14 (Bulging squares; IntroCS)
*
* Dependencies: StdDraw.java
*
* Compilation: javac BulgingSquare.java
* Execution:   java BulgingSquare
***************************************************************/

public class BulgingSquare
{
  public static void main (String[] args)
  {
    /* Define as escalas x e y indo de -7.5 a 7.5 */
    /* Todos os quadrados são desenhados por quadrantes, de forma que é necessário apenas mudar o sinal de i e/ou j para que, a cada passo, a figura seja formada */
    StdDraw.setScale (-7.5, 7.5);
    double i, j, k;
    double aux = .15;

    /* Desenha os quadrados maiores */
    StdDraw.setPenColor ();
    for (i = -7; i <= 0; i++)
      for (j = -7; j <= 0; j++)
        if ((i + j) % 2 == 0)
        {
          StdDraw.filledSquare (i, -j, 0.5);
          StdDraw.filledSquare (-i, j, 0.5);
          StdDraw.filledSquare (i, j, 0.5);
          StdDraw.filledSquare (-i, -j, 0.5);
        }

    /* Desenha alguns dos quadrados menores */
    j = 0;
    for (i = -6; i < 0; i++)
    {
      if ((i + j) % 2 == 0)
        StdDraw.setPenColor (StdDraw.WHITE);
      else
        StdDraw.setPenColor (StdDraw.BLACK);

      StdDraw.filledSquare (i + 2 * aux, j + 2 * aux, aux);
      StdDraw.filledSquare (i + 2 * aux, j - 2 * aux, aux);

      StdDraw.filledSquare (j + 2 * aux, i + 2 * aux, aux);
      StdDraw.filledSquare (j - 2 * aux, i + 2 * aux, aux);

      StdDraw.filledSquare (-i - 2 * aux, -j + 2 * aux, aux);
      StdDraw.filledSquare (-i - 2 * aux, -j - 2 * aux, aux);

      StdDraw.filledSquare (-j + 2 * aux, -i - 2 * aux, aux);
      StdDraw.filledSquare (-j - 2 * aux, -i - 2 * aux, aux);
    }

    /* Desenha mais alguns quadrados menores */
    k = 0;
    for (j = -1; j > -7 ; j--, k++)
    {
      for (i = -1; i > -7 + k; i--)
      {
        if ((i + j) % 2 == 0)
          StdDraw.setPenColor (StdDraw.WHITE);
        else
          StdDraw.setPenColor (StdDraw.BLACK);

        /* Desenha os quadrados menores por quadrantes */
        StdDraw.filledSquare (i - 2 * aux, j + 2 * aux, aux);
        StdDraw.filledSquare (i + 2 * aux, j - 2 * aux, aux);

        StdDraw.filledSquare (-i - 2 * aux, -j + 2 * aux, aux);
        StdDraw.filledSquare (-i + 2 * aux, -j - 2 * aux, aux);

        StdDraw.filledSquare (-j + 2 * aux, i + 2 * aux, aux);
        StdDraw.filledSquare (-j - 2 * aux, i - 2 * aux, aux);

        StdDraw.filledSquare (j + 2 * aux, -i + 2 * aux, aux);
        StdDraw.filledSquare (j - 2 * aux, -i - 2 * aux, aux);
      }
    }

    /* Desenha os últimos quadrados menores */
    for (k = 0; k < 3; k++)
    {
      i = -5 + k;
      j = -3 - k;

      if ((i + j) % 2 == 0)
        StdDraw.setPenColor (StdDraw.WHITE);
      else
        StdDraw.setPenColor (StdDraw.BLACK);

      StdDraw.filledSquare (i - 2 * aux, j + 2 * aux, aux);
      StdDraw.filledSquare (i + 2 * aux, j - 2 * aux, aux);

      StdDraw.filledSquare (-i - 2 * aux, -j + 2 * aux, aux);
      StdDraw.filledSquare (-i + 2 * aux, -j - 2 * aux, aux);

      StdDraw.filledSquare (-j + 2 * aux, i + 2 * aux, aux);
      StdDraw.filledSquare (-j - 2 * aux, i - 2 * aux, aux);

      StdDraw.filledSquare (j + 2 * aux, -i + 2 * aux, aux);
      StdDraw.filledSquare (j - 2 * aux, -i - 2 * aux, aux);
    }

    StdDraw.show ();
  }
}
