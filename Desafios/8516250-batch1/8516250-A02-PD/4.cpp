/*
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
MAC0327 - Desafios de Programação - 2016/2
Batch 1 - Aula 02 - Problema D
*/

#include <bits/stdc++.h>
using namespace std;

int main()
{
  int N, K, done = 1, t = 0, k = 1;
  scanf ("%d %d", &N, &K);

  if (K < N)
  {
    t = (int) ceil(log2(K));
    done = (int) pow(2, t);
    t += (int) ceil((double) (N - done) / K);
  }
  else
    t = (int) ceil(log2(N));

  printf("%d", t);
}
