#include <bits/stdc++.h>
using namespace std;

int gcd(int a, int b)
{
  return b == 0 ? a : gcd(b, a % b);
}

int main()
{
  int T, i, j, A, B, C, D, AC, BD, aux;
  double a, b, x0, y0;
  scanf("%d", &T);

  int cr[T];
  for (i = 0; i < T; i++)
    cr[i] = 1;

  for (i = 0; i < T; i++)
  {
    scanf("%d %d %d %d", &A, &B, &C, &D);

    AC = abs(A - C);
    BD = abs(B - D);

    if (AC == 0)      cr[i] += BD;
    else if (BD == 0) cr[i] += AC;
    else
    {
      aux = gcd(AC, BD);
      cr[i] += aux;
    }
  }

  for (i = 0; i < T; i++)
    printf("%d\n", cr[i]);
}
