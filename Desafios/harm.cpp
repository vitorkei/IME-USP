#include <bits/stdc++.h>
using namespace std;

int main()
{
  int n, i, j, T;
  double count, ln;
  scanf("%d", &T);

  for (j = 0; j < T; j++)
  {
    scanf("%d", &n);
    printf("%d - ", n);
    count = 0;
    for (i = 2; i < n/2 + 1; i++)
      count += 1.0/i;
    ln = (log(((double) n)/2 + 1));
    printf("count = %f || ln = %f\n", count, ln);
  }
}
/*
54
2 - 27
3 - 18
4 - 13
5 - 10
6 - 9
7 - 7
8 - 6
9 - 6
10 - 5
11 - 4
12 - 4
13 - 4
14 - 3
15 - 3
16 - 3
17 - 3
18 - 3
19 - 2
20 - 2
21 - 2
22 - 2
23 - 2
24 - 2
25 - 2
26 - 2
27 - 2
*/
