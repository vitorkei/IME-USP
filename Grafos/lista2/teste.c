#include <stdlib.h>
#include <stdio.h>
#define vertex int

typedef struct node *link;
struct node
{
  vertex w;
  link next;
};

struct graph
{
  int V;
  int A;
  link *adj;
};
typedef struct graph *Graph;

int main()
{
  printf("oi\n");
  return 0;
}
