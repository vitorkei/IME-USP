#include <stdio.h>
#include <string.h>

int main()
{
  char input[51];
  scanf("%s", input);
  int len;
  len = strlen(input);

  char num[len];
  int i;
  for (i = 0; i < len; i++)
    num[i] = input[i];

  i = 0;

  int aux0, aux1;
  aux0 = (int) num[i];
  aux0 -= 48;
  if (aux0 < 7)
  {
    aux1 = (int) num[i++];
    aux1 -= 48;
    aux0 %= 7;
  }
  while (i < len)
  {
    aux1 = (int) num[i];
    aux1 -= 48;
    aux0 = aux0 * 10 + aux1;

    aux0 %= 7;

    i++;
  }

  printf("%d", aux0);

  return 0;
}
