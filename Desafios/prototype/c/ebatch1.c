#include <stdio.h>

int main()
{
  int i, n;
  int pos = 0, neg = 0, zero = 0;
  scanf("%d", &n);

  int nums[n];
  for (i = 0; i < n; i++)
  {
    scanf("%d", &nums[i]);
    if      (nums[i] > 0) pos++;
    else if (nums[i] < 0) neg++;
    else                  zero++;
  }

  int pos_pos[pos];
  int neg_pos[neg];
  int zero_pos[zero];

  

  return 0;
}
