#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define vertex int
#define UGraph Graph

/* Estrutura */
typedef struct node *link;
struct node{
  vertex w;
  link next;
};
struct graph{
  int V; /* Vertices */
  int E; /* Arestas  */
  link *adj;
};
typedef struct graph *Graph;

static link NEWnode (vertex w, link next){
  link a = malloc (sizeof (struct node));
  a->w = w;
  a->next = next;
  return a;
}

/* Inicializa um grafo não-dirigido por lista de adjacências com V vértices */
UGraph UGRAPHinit (int V)
{
  vertex v;
  UGraph UG = malloc (sizeof *UG);
  UG->V = V;
  UG->E = 0;
  UG->adj = malloc (V * sizeof (link));
  for (v = 0; v < V; v++) UG->adj[v] = NULL;
  
  return (UG);
}

/* Insere um arco no grafo */
void UGRAPHinsertEdge (UGraph UG, vertex v, vertex w)
{
  link a;
  
  for (a = UG->adj[v]; a != NULL; a = a->next)
    if (a->w == w)
      return;
  UG->adj[v] = NEWnode (w, UG->adj[v]);
  
  for (a = UG->adj[w]; a != NULL; a = a->next)
    if (a->w == v)
      return;
  UG->adj[w] = NEWnode (v, UG->adj[w]);
  
  UG->E++;
}

/* Imprime o grafo */
void UGRAPHshow (UGraph UG)
{
  vertex v;
  link p;
  for (v = 0; v < UG->V; v++)
  {
    printf ("%d: ", v);
    for (p = UG->adj[v]; p != NULL; p = p->next)
      printf ("%d ", p->w);
    printf ("\n");
  }
  printf("UG->E = %d\n", UG->E);
}

/*
 * Gera um grafo não-dirigido aleatório com V
 * vértices e número esperado de E arestas
 */
UGraph UGRAPHrand (vertex V, int E)
{
  vertex v, w;
  double p = (double) 2 * E / (V * (V-1));
  UGraph UG = UGRAPHinit (V);
  
  for (v = 0; v < V; v++)
    for (w = 0; w < V; w++)
      if (v != w)
        if (rand() < p * (RAND_MAX + 1.0))
          UGRAPHinsertEdge (UG, v, w);
  
  return (UG);
}

int main()
{
  UGraph UG;
  
  srand (time (NULL));
  
  UG = UGRAPHrand(10, 2);
  UGRAPHshow (UG);
  return 0;
}
