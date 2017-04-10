/*
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
MAC0327 - Desafios de Programação - 2016/2
Batch 1 - Aula 01 - Problema C
*/

#include <bits/stdc++.h>
using namespace std;

int main()
{
  int n, i;
  scanf("%d", &n);

  char str[n];
  int letter[26];
  scanf("%s", str);

  for (i = 0; i < 26; i++) letter[i] = 0;

  for (i = 0; i < n; i++)
  {
    int j = (int) str[i];

    if (j < 91)
    {
      j -= 65;
      letter[j] = 1;
    }
    else
    {
      j -= 97;
      letter[j] = 1;
    }
  }

  int flag = 1;
  for (i = 0; i < 26 && flag == 1; i++)
    if (letter[i] == 0) flag = 0;

  if (flag == 1) printf("YES");
  else           printf("NO");

  return 0;
}
