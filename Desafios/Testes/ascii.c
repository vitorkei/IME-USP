#include <stdio.h>

int main()
{
  char c[3];
  scanf("%s", c);
  int i;
  for (i = 0; i < 3; i++)
  {
    int j = (int) c[i];
    printf("%d ", j);
  }

  return 0;
}
