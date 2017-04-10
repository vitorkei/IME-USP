#include <stdio.h>

int dist(char a, char b)
{
  int res = 0;
  int auxa = (int) a;
  int auxb = (int) b;

  if      (auxa > auxb) res = auxa - auxb;
  else if (auxa < auxb) res = auxb - auxa;

  return res;
}

int main()
{
  int n, k;
  scanf("%d", &n);
  scanf("%d", &k);

  char str[n], strp[n];
  scanf("%s", str);

  int i;
  for (i = 0; i < n; i++)
  {
    printf("k = %d\n", k);
    int let = (int) str[i];
    let -= 97;
    if (let > 12)
    {
      if (k > 25 - let)
      {
        k -= 25 - let;
    printf("k' = %d\n", k);
        strp[i] = 'z';
      }
      else
      {
        let += k + 97;
        strp[i] = (char) let;
      }
    }
    else
    {
      if (k > let)
      {
        k -= let;
        strp[i] = 'a';
      }
      else
      {
        let -= k;
        let += 97;
        strp[i] = (char) let;
      }
    }
  }

  if (k == 0) printf("%s", strp);
  else if (k > 0) printf("-1");
  else printf("ERRO");

  return 0;
}
