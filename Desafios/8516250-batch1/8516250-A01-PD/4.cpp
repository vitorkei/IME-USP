/*
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
MAC0327 - Desafios de Programação - 2016/2
Batch 1 - Aula 01 - Problema D
*/

#include <bits/stdc++.h>
using namespace std;

int main()
{
  long long i, N, m, h;
  double j;
  scanf("%lld", &N);

  int nums[N];

  for (i = 0; i < N; i++)
  {
    scanf("%lld", &m);
    m = (m - 1) * 8 + 1;
    j = sqrt(m);
    if (floor(j) == ceil(j))
    {
      if ((((int) j) - 1) % 2 == 0) nums[i] = 1;
      else                          nums[i] = 0;
    }
    else nums[i] = 0;
  }

  for (i = 0; i < N - 1; i++) printf("%d ", nums[i]);
  printf("%d", nums[N - 1]);

  return 0;
}
