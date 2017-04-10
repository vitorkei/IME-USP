/*
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
MAC0327 - Desafios de Programação - 2016/2
Batch 1 - Aula 01 - Problema F
*/

#include <bits/stdc++.h>
using namespace std;

int main()
{
  int n, k, i, l;
  scanf("%d %d", &n, &k);

  char str[n], strl[n];
  scanf("%s", str);

  for (i = 0; i < n; i++)
  {
    l = max ('z' - str[i], str[i] - 'a');
//    printf("\nk = %d\nl = %d\n", k, l);

    if (k > l)
    {
//      printf("k > l\n");
      k -= l;
//      printf("k' = %d\n", k);
      if (str[i] < 'n') strl[i] = 'z';
      else              strl[i] = 'a';
    }
    else if (k == l)
    {
//      printf("k == l\n");
      if (str[i] - k > 96) strl[i] = str[i] - k;
      else                 strl[i] = str[i] + k;
      k = 0;
    }
    else if (k == 0)
    {
//      printf("k == 0\n");
      strl[i] = str[i];
    }
    else
    {
//      printf("k < l\n");
      l -= k;
//      printf("l' = %d\n", l);
      if (str[i] < 'n') strl[i] = (char) str[i] + k;
      else              strl[i] = (char) str[i] - k;
      k = 0;
    }
  }

  if (k == 0)
    for (i = 0; i < n; i++)
      printf("%c", strl[i]);
  else
    printf("-1");
}
