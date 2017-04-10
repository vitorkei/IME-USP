/*
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
MAC0327 - Desafios de Programação - 2016/2
Batch 1 - Aula 04 - Problema C
*/

#include <bits/stdc++.h>
using namespace std;

long long g(long long n, int x, int A, int B, long long lim)
{
  if (n == 0) return x;
  long long y = g(n - 1, x, A, B, lim);
  long long f = A * y + B;
  return f;
}

int main()
{
  long long A, B, x, i, n, resp, m, a, b;

  scanf("%lld %lld %lld %lld", &A, &B, &n, &x);
  m = pow(10, 9) + 7;

printf("A = %lld || B = %lld || n = %lld || x = %lld\n", A, B, n, x);
  a = B;
  b = B;
printf("a = %lld || b = %lld\n", a, b);
  for (i = 0; i < n; i++)
  {
    b *= A;
    a += b;
printf("a' = %lld || b' = %lld\n", a, b);
    if (a >= m) a %= m;
  }
  b /= B;
  b *= A;
  if (b >= m) b %= m;
  b *= x;
  if (b >= m) b %= m;
  a += b;
  if (a >= m) a %= m;

  printf("%lld\n", a);
}
