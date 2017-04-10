/*
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
MAC0327 - Desafios de Programação - 2016/2
Batch 1 - Aula 02 - Problema A
*/

#include <bits/stdc++.h>
using namespace std;

int main()
{
  int n, l, i, dist = 0, lim_dist;
  double distf;
  scanf("%d %d", &n, &l);

  int loc[n];
  for (i = 0; i < n; i++)
    scanf("%d", &loc[i]);

  stable_sort(loc, loc + n);

  if (loc[0] > l - loc[n - 1]) lim_dist = loc[0];
  else                         lim_dist = l - loc[n - 1];

  for (i = 0; i < n - 1; i++)
    if (loc[i + 1] - loc[i] > dist)
      dist = loc[i + 1] - loc[i];

  if (2 * lim_dist > dist)
    distf = (double) lim_dist;
  else
  {
    distf = (double) dist;
    distf /= 2;
  }

  printf("%.10f\n", distf);
}
