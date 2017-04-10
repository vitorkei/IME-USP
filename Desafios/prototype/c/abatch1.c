#include <stdio.h>

int main ()
{
  int t, s, x, n;
  scanf ("%d %d %d", &t, &s, &x);

  n = (x - t) / s;
  if (n == 0) n = 1;

  if      (x < t)              printf("NO");
  else if (x == t)             printf("YES");
  else if (x < t + s * n)      printf("NO");
  else if (x == t + s * n)     printf("YES");
  else if (x == t + s * n + 1) printf("YES");
  else                         printf("NO");

  return 0;
}
