/*
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
MAC0327 - Desafios de Programação - 2016/2
Batch 1 - Aula 01 - Problema E
*/

#include <bits/stdc++.h>
using namespace std;

int main()
{
  int m, i, p, z, n, used[3], j;
  scanf("%d", &m);

  int nums[m];
  for (i = 0, p = 0, z = 0, n = 0; i < m; i++)
  {
    scanf("%d", &nums[i]);
    if      (nums[i] > 0)  p++;
    else if (nums[i] == 0) z++;
    else                   n++;
  }

  j = 0;
  for (i = 0; i < m; i++)
    if (nums[i] < 0)
    {
      printf("1 %d\n", nums[i]);
      used[j] = nums[i];
      break;
    }

  if (p == 0)
  {
    printf("2 ");
    for (i = 0; i < m && p < 2; i++)
      if (nums[i] < 0 && nums[i] != used[0])
      {
        printf("%d ", nums[i]);
        j++;
        used[j] = nums[i];
        p++;
      }
    printf("\n");
    p = 0;
  }
  else
  {
    for (i = 0; i < m; i++)
      if (nums[i] > 0)
      {
        printf("1 %d\n", nums[i]);
        used[1] = nums[i];
        break;
      }
  }

  if (p == 0)
  {
    printf("%d ", (m - 3));
    for (i = 0; i < m; i++)
      if (nums[i] != used[0] && nums[i] != used[1] && nums[i] != used[2])
        printf("%d ", nums[i]);
  }
  else
  {
    printf("%d ", (m - 2));
    for (i = 0; i < m; i++)
      if (nums[i] != used[0] && nums[i] != used[1])
        printf("%d ", nums[i]);
  }

  return 0;
}
