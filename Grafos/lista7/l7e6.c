#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define vertex int
#define maxV 1000

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

/* Protótipos */
static link   NEWnode             (vertex, link);
       Graph Graphinit          (int);
       void   GraphinsertEdge    (Graph, vertex, vertex);
       void   Graphshow          (Graph);
       Graph Graphrand          (vertex, int);

/* Cria um novo nó para a lista de adjacências */
static link NEWnode (vertex w, link next){
  link a = malloc (sizeof (struct node));
  a->w = w;
  a->next = next;
  return a;
}

/* Inicializa um grafo não-dirigido por lista de adjacências com V vértices */
Graph Graphinit (int V)
{
  vertex v;
  Graph G = malloc (sizeof *G);
  G->V = V;
  G->E = 0;
  G->adj = malloc (V * sizeof (link));
  for (v = 0; v < V; v++) G->adj[v] = NULL;
  
  return (G);
}

/* Insere um arco no grafo */
void GraphinsertEdge (Graph G, vertex v, vertex w)
{
  link a;
  
  for (a = G->adj[v]; a != NULL; a = a->next)
    if (a->w == w)
      return;
  G->adj[v] = NEWnode (w, G->adj[v]);
  
  for (a = G->adj[w]; a != NULL; a = a->next)
    if (a->w == v)
      return;
  G->adj[w] = NEWnode (v, G->adj[w]);
  
  G->E++;
}

/* Imprime o grafo */
void Graphshow (Graph G)
{
  vertex v;
  link p;
  printf ("Arestas:\n");
  for (v = 0; v < G->V; v++)
  {
    printf ("%d: ", v);
    for (p = G->adj[v]; p != NULL; p = p->next)
      printf ("%d ", p->w);
    printf ("\n");
  }
  printf("G->E = %d\n\n", G->E);
}

/*
 * Gera um grafo não-dirigido aleatório com V
 * vértices e número esperado de E arestas
 */
Graph Graphrand (vertex V, int E)
{
  vertex v, w;
  double p = (double) 2 * E / (V * (V-1));
  Graph G = Graphinit (V);
  
  for (v = 0; v < V; v++)
    for (w = 0; w < V; w++)
      if (v != w)
        if (rand() < p * (RAND_MAX + 1.0))
          GraphinsertEdge (G, v, w);
  
  return (G);
}

int main (int argc, char *argv[])
{
  int V, E;
  Graph G;
  
  V = atoi(argv[1]);
  E = atoi(argv[2]);
  
  G = Graphrand (V, E);
  Graphshow (G);
  
  return 0;
}
