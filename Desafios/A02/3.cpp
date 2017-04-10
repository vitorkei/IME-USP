#include <bits/stdc++.h>
using namespace std;

int main()
{
  long long C, T, i, j, x, y, p = 0;
  long double r, x0, y0;
  scanf("%lld %lld", &C, &T);

  long long rs[C];
  for (i = 0; i < C; i++)
    scanf("%lld", &rs[i]);

  for (i = 0; i < T; i++)
  {
    scanf("%lld %lld", &x, &y);
    x0 = ((long double) x) / 100;  // Se x0 < 100, fica menor que 1 e só diminui mais ou fazer ao quadrado
    y0 = ((long double) y) / 100;  //
    r = 100 * ((long double) sqrt(x0 * x0 + y0 * y0));
    for (j = 0; j < C; j++)
    {
      x0 = (long double) rs[j];
      if (r <= x0)
      {
        p += C - j;
        break;
      }
    }
  }

  printf("%lld\n", p);
}
