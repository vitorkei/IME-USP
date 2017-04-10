/*
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
MAC0327 - Desafios de Programação - 2016/2
Batch 1 - Aula 03 - Problema B
*/

#include <bits/stdc++.h>
using namespace std;

int gcd(int a, int b)
{
  return b == 0 ? a : gcd(b, a % b);
}

int main()
{
  int T, N, t, i, j, count = 0, a, last = 1;
  scanf("%d", &T);

//  static int cp[100001][100001];
//  memset(cp, 0, 100001*100001);
  int cp[100001];
  memst(cp, 0, 100001);

  for (t = 0; t < T; t++)
  {
    scanf("%d", &N);

    if (N + 1 > last)
    {
      for (i = last; i < N + 1; i++)
      {
        for (j = 1; j < i + 1; j++)
          if (gcd(i, j) > 1)
          {
            count++;
            cp[i] = count;
          }
      }
      last = N;
      a = count;
    }
    else
    {
      
    }

    printf("Case %d: %d\n", (t + 1), count);
  }
/*
count = 0;
//    if (N % 2 == 0) count = N / 2;
//    else            count = N / 2 + 1;
//printf("\ncount = %d\n", count);
    for (i = 2; i < N + 1; i++)
    {
      for (j = i; j < N + 1; j++)
      {
//printf("i = %d || j = %d || ", i, j);
        if (cp[i][j] == 0)
        {
//printf("if || ");
          cp[i][j] = gcd(i, j);
          if (cp[i][j] > 1)
          {
            count++;
//printf("count++");
          }
        }
        else if (cp[i][j] > 1)
        {
          count++;
//printf("count++");
        }
//printf("\n");
//printf("count' = %d\n", count);
      }
    }
*/
}

