/*
Nome: Vítor Kei Taira Tamada
NUSP: 8516250
MAC0327 - Desafios de Programação - 2016/2
Batch 1 - Aula 02 - Problema E
*/

#include <bits/stdc++.h>
using namespace std;

int main()
{
  int len, i, j, k, num[100], x, flag = 0;
  char c = '0';
  for (len = 0; c > 47 && c < 58; len++)
  {
    scanf("%c", &c);
    num[len] = (int) c - 48;
  }
  len--;

  if (len == 1)
  {
    if (num[0] % 8 == 0) printf("YES\n%d", num[0]);
    else                 printf("NO");
  }
  else if (len == 2)
  {
    x = num[0] + num[1]*10;
    if      (x % 8 == 0)      printf("YES\n%d", x);
    else if (num[0] % 8 == 0) printf("YES\n%d", num[0]);
    else if (num[1] % 8 == 0) printf("YES\n%d", num[1]);
    else                      printf("NO");
  }
  else
  {
    for (i = 1; i < len - 1 && flag == 0; i++)
    {
//    printf("i = %d\nlen - i = %d\n", i, (len - i));
      for (j = i + 1; j < len && flag == 0; j++)
      {
//      printf("\nj = %d\nlen - j = %d\n", j, len - j);
        for (k = j + 1; k < len + 1 && flag == 0; k++)
        {
//        printf("\nk = %d\nlen - k = %d\n", k, len - k);
          x = num[len - i] + num[len - j]*10 + num[len - k]*100;
//        printf("\ni = %d\nj = %d\nk = %d\n", i, j, k);
//        printf("x = %d\n", x);
          if (x % 8 == 0)
          {
            printf("YES\n");
            flag = 1;
//          printf("len - i = %d\nlen - j = %d\nlen - k = %d\n", (len-i), (len-j), (len-k));
          }
        }
      }
    }
    if   (flag == 1) printf("%d", x);
    else             printf("NO");
  }
}
