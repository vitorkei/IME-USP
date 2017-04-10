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

    if (k > l)
    {
      k -= l;
      if (str[i] < 'n') strl[i] = 'z';
      else              strl[i] = 'a';
    }
    else if (k == l)
    {
      k = 0;
      if (str[i] < 'n') strl[i] = 'a';
      else              strl[i] = 'z';
    }
    else if (k == 0)
      strl[i] = str[i];
    else
    {
      l -= k;
      if (str[i] < 'n') strl[i] = (char) str[i] + k;
      else              strl[i] = (char) str[i] - k;
      k = 0;
    }
  }

  int sum;
  for (i = 0, sum = 0; i < n; i++)
  {
    sum += abs((int) strl[i] - str[i]);
    printf("%c(%d) - %c(%d) = %d\nsum = %d\n\n", str[i], ((int) str[i] - 97), strl[i], ((int) strl[i] - 97), abs((int) strl[i] - str[i]), sum);
  }

  if (k == 0)
    for (i = 0; i < n; i++)
      printf("%c", strl[i]);
  else
    printf("-1");
}
