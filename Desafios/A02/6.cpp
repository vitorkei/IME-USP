#include <bits/stdc++.h>
using namespace std;

int main()
{
  long long n, i, o;
  scanf("%lld", &n);

  long long nums[n];
  for (i = 0; i < n; i++)
  {
    scanf("%lld", &nums[i]);
    if (nums[i] == n)
    {
      printf("1\n%lld", nums[i]);
      break;
    }
  }

  for (i = 0; i < n; i++)
    printf("%lld ", nums[i]);
}
