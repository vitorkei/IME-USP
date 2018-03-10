#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
  int i, j, n, N, count;
  int *X, *rep;
  printf("Insira N:");
  scanf("%d", &N);
  N++;
  
  X = malloc(N*sizeof(int));
  rep = malloc(N*sizeof(int));
  for(i = 0; i < N; i++) X[i] = 0;
  
  srand(time(NULL));
  for(j = 0; j < 100; j++)
  {
    count = 0;
    for(i = 0; i < N; i++) rep[i] = 0;
    for(i = 0; i < N-1; i++)
    {
      n = rand() % (N-1);
      while(rep[n]) n = rand() % (N-1);
      rep[n] = 1;
      printf("(%d) n = %d\n", i, n);
      if (n == i) count++;
    }
    printf("iteração = %d || count = %d\n\n", j, count);
    X[count]++;
  }
  
  for (i = 0; i < N; i++)
    if(X[i] != 0)
      printf("P(X = %d) = %d/100\n", i, X[i]);
  
  free(X);
  free(rep);
  
  return 0;
}
