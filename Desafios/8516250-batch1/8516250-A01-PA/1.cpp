/*
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
MAC0327 - Desafios de Programação - 2016/2
Batch 1 - Aula 01 - Problema A
*/

#include <bits/stdc++.h>
using namespace std;

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
