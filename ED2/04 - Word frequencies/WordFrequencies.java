/***************************************************************
* MAC0323 - Algoritmos e Estruturas de Dados II
*
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* Creative Exercise 3.1.52 (Word frequencies; IntroCS)
*
***************************************************************/

public class WordFrequencies
{
  public static void main (String[] args)
  {
    // Declaração de variáveis
    String[] text = StdIn.readAllStrings ();  // No caso deste programa, text recebe o texto que Quick3way.java devolveu
    String[] list = new String [text.length]; // list receberá o text, sem as repetições (as últimas casas de list serão iguais a null)
    int[] count = new int [text.length];      // count[i] é o número de vezes que a palavra list[i] aparece no texto. Se list[i] == null, então count[i] == 0
    int i, j;                                 // Variáveis auxiliares
    boolean flag;

    // Preenche list e count com seus devidos valores (como descritos acima)
    for (i = 0; i < text.length; i++)
    {
      flag = false;
      String word = text[i];

      // Verifica se a palavra já apareceu no texto antes
      for (j = 0; j < list.length && !flag; j++)
      {
        if (word.equals (list[j]))  // Caso tenha aparecido, aumenta em 1 sua frequência e sai do for
        {
          count[j]++;
          flag = true;
        }
        else if (list[j] == null)   // Caso contrário, adiciona à lista e sai do for
        {
          list[j] = word;
          count[j]++;
          flag = true;
        }
      }
    }

    // sortedCount[] é um vetor que contém o valor das contagens de cada palavra em ordem descrescente
    // Como sortedCount[] tem o mesmo tamanho que count[], suas últimas casas serão sempre iguais a 0
    int[] sortedCount = new int [count.length];

    for (i = 0; count[i] != 0; i++)  // Primeira casa de sortedCount[]
      if (sortedCount[0] < count[i])
        sortedCount[0] = count[i];

    for (i = 1; i < sortedCount.length; i++)  // Demais casas do vetor
      for (j = 0; count[j] != 0; j++)
        if (sortedCount[i] < count[j] && count[j] < sortedCount[i - 1])
          sortedCount[i] = count[j];

    // Imprime as palavras em, primeiro, ordem de frequência e, em segundo, ordem alfabética
    for (i = 0; sortedCount[i] != 0; i++)
      for (j = 0; count[j] != 0; j++)  // Poderia fazer list[j] != null, o que não alteraria o funcionamento do laço
        if (count[j] == sortedCount[i])
          StdOut.println (list[j] + " " + count[j]);
  }
}
