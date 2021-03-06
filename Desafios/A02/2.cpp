/*
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
MAC0327 - Desafios de Programação - 2016/2
Batch 1 - Aula 02 - Problema B
*/

#include <bits/stdc++.h>
using namespace std;

int main()
{
  int n, i, j, x, count, root, pri[10000];
  scanf("%d", &n);

  for (i = 0; i < 10000; i++)
    pri[i] = 0;

  for (j = 2; j <= n; j++)
  {
    x = j;
    count = 0;
    while (x % 2 == 0)
    {
      pri[2]++;
      x /= 2;
    }

    if (x != 1)
    {
      root = (int) sqrt(x);
      for (i = 3; i <= root; i += 2)
      {
        count = 0;
        while (x % i == 0)
        {
          pri[i]++;
          x /= i;
        }
      }

      if (x > 2)
        pri[x]++;
    }
  }

  printf("2^%d", pri[2]);
  for (i = 3; i < 10000; i++)
    if (pri[i] != 0)
      printf(" * %d^%d", i, pri[i]);
}
