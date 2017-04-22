/*
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
MAC0327 - Desafios de Programação - 2016/2
Batch 1 - Aula 03 - Problema D
*/

/*
erro 23 codeforces
*/

#include <bits/stdc++.h>
using namespace std;

int main()
{
  long long a = 1, lim, i, j, M, p, exp = 1, high = 0, mid = 1, out, m, e, b;
  scanf("%I64d", &M);

  long long primes[200000];
  for (i = 2; i < 200000; i++)
    primes[i] = 0;

  lim = 1000000007;
  for (i = 0; i < M; i++)
  {
    scanf("%I64d", &p);

    if (p > high) high = p;

    primes[p]++;
  }

  high++;

  for (i = 2; i < 200000; i++)
    if (primes[i] > 0)
      exp *= (primes[i] + 1);

  if (exp % 2 != 0)
  {
    for (i = 2; i < 200000; i++)
      if (primes[i] > 0)
      {
        e = primes[i] / 2;
        b = i;
        out = 1;
        while (e > 0)
        {
          while (e % 2 == 0)
          {
            e /= 2;
            b = (b*b) % lim;
          }
          if (e != 1) out = (out*b) % lim;
          e--;
        }
        b = (b*out) % lim;
        mid = (mid * b) % lim;
      }
    exp--;
  }
  exp = exp / 2;

  for (i = 2; i < 200000; i++)
    if (primes[i] > 0)
    {
      e = primes[i] * exp;
      b = i;
      out = 1;
      while (e > 0)
      {
        while (e % 2 == 0)
        {
          e /= 2;
          b = (b*b) % lim;
        }
        if (e != 1) out = (out*b) % lim;
        e--;
      }
      b = (b*out) % lim;
      a = (a*b) % lim;
    }
  a = (a * mid) % lim;
  printf("%I64d\n", a);
}