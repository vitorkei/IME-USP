/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Creative Exercise 3.2.47 (Random walker; IntroCS)
*
***************************************************************/

public class Distribution
{
  public static void main (String[] args)
  {
    int N = Integer.parseInt (args[0]);   // Número de passos dados por cada Walker
    int M = Integer.parseInt (args[1]);   // Número de Walkers
    int L = Integer.parseInt (args[2]);   // Metade do lado do quadrado que será impresso
    int max = 0;    // Número máximo de visitas em uma célula
    int[][] grid = new int [2*N + 1][2*N + 1];    // Grid que contém o número de vezes que cada célula é visitada. Para as dimensões, vide README

    // Define escala do quadrado que será desenhado
    StdDraw.setScale (-L, L);

    // N passos dos M RandomWalkers
    // Contagem de quantas vezes cada célula foi visitada
    for (int i = 0; i < M; i++)
    {
      RandomWalker rw = new RandomWalker (0, 0);
      for (int j = 0; j < N; j++)
      {
        rw.step ();
        grid[rw.x + N][rw.y + N]++;
      }
    }

    // Define a célula com maior número de visitas
    for (int i = 0; i < 2*N + 1; i++)
      for (int j = 0; j < 2*N + 1; j++)
        if (grid[i][j] > max)
          max = grid[i][j];

    // Desenha o plano em que os Walkers andam
    // Azul claro define os espaços que não foram visitados nenhuma vez
    // Azul escuro define os espaços que foram visitadospoucas vezes
    // Vermelho/avermelhado define os espaços que foram visitados mais vezes
    // Espaço em branco é a área fora do plano em que os Walkers andam (espaço "extra" pelo fato de o L ser muito grande)
    for (int i = 0; i < 2*N + 1; i++)
      for (int j = 0; j < 2*N + 1; j++)
      {
        if (grid[i][j] != 0 )
        {
          int color = (int) ((double)grid[i][j] / max * 255);
          StdDraw.setPenColor (color, 0, 255 - color);
          StdDraw.filledSquare (i - N, j - N, .5);
        }
        else
        {
          StdDraw.setPenColor (0, 255, 255);
          StdDraw.filledSquare (i - N, j - N, .5);
        }
      }
  }
}
