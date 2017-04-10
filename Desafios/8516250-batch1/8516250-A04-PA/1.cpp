/*
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
MAC0327 - Desafios de Programação - 2016/2
Batch 1 - Aula 04 - Problema A
*/

#include <bits/stdc++.h>
using namespace std;

int main()
{
  long long e, b, m, i, T, out, aux;
  scanf("%lld", &T);

  for (i = 0; i < T; i++)
  {
    out = 1;
    aux = 0;
    scanf("%lld %lld %lld", &b, &e, &m);
    if (b >= m) b = b % m;
    while (e > 0)
    {
      if (out > m)
        out %= m;
      while (e % 2 == 0)
      {
        e /= 2;
        b = (b*b) % m;
      }
      if (e != 1) out *= b;
      e--;
    }
    b *= out;
    b %= m;
    printf("%lld. %lld\n", (i + 1), b);
  }
}
