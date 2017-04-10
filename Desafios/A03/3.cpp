/*
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
MAC0327 - Desafios de Programação - 2016/2
Batch 1 - Aula 03 - Problema C
*/

/*
Para N = 5 e K = 1, o intervalo [4, 5] não é pego
*/

#include <bits/stdc++.h>
using namespace std;

int main()
{
  int p = 0, i, j, root;

  root = (int) sqrt(100001);
  root++;

  int prime[100001];
  for (i = 2; i < 100001; i++)
    prime[i] = 1;

  for (i = 2; i < root; i++)
  {
    if (prime[i] == 1)
      for (j = i * i; j < 100001; j += i)
        prime[j] = 0;
  }

/***************************************/

  int T, N, K, k, n, m;
  scanf("%d", &T);

  int resp[T];

  for (i = 0; i < T; i++)
  {
    scanf("%d %d", &N, &K);

    if (N == 2)
    {
      if (K > 1) resp[i] = 0;
      else       resp[i] = 1;
      continue;
    }

    p = k = m = 0;
    n = N + 1;
    for (j = 2; j < n; j++)
    {
      printf("\nj = %d\n", j);
      if (k != K)
      {
        if (prime[j] == 1) k++;
        if (k == K)        p = m = 1;
        printf("k = %d || p = %d || m = %d\n", k, p, m);
      }
      else
      {
        if (prime[j] == 1) m++;
        p += m;
        printf("p = %d\n", p);
      }
    }
    resp[i] = p;
    printf("\n/******/\n\n");
  }
  for (i = 0; i < T; i++)
    printf("%d\n", resp[i]);
}
