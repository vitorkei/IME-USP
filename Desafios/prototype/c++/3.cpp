#include <bits/stdc++.h>
using namespace std;

int main()
{
  int C, T, i, j, p = 0;
  double r, x, y;
  scanf("%d %d", &C, &T);

  double rs[C];
  for (i = 0; i < C; i++)
    scanf("%f", &rs[i]);

  for (i = 0; i < T; i++)
  {
    scanf("%f %f", &x, &y);
    x /= 100;
    y /= 100;
    r = 100 * sqrt(x * x + y * y);
    for (j = 0; j < C; j++)
    {
      if (r <= rs[j])
      {
        p += C - j;
        break;
      }
    }
  }
  printf("%d\n", p);
}
