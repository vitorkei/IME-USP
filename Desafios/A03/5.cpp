/*
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
MAC0327 - Desafios de Programação - 2016/2
Batch 1 - Aula 03 - Problema E
*/

#include <bits/stdc++.h>
using namespace std;

int main()
{
  int i, j, k, a[10], num[10001], max = 0, count = 1;

  for (i = 0; i < 10001; i++)
    num[i] = 0;
  num[1] = 1;

  for (i = 0; i < 10; i++)
  {
    scanf("%d", &a[i]);
    if (a[i] >= max) max = a[i] + 1;
    k = 2;
    while (a[i] != 1)
    {
      j = k;
      while (a[i] % j == 0)
      {
        num[j]++;
        a[i] /= j;
      }
      if (k % 2 == 0) k++;
      else            k += 2;
    }
  }
  for (i = 2; i < max; i++)
  {
    if (num[i] > 0)
      count *= (num[i] + 1);
  }
  count = count % 10;
  printf("%d\n", count);
}
