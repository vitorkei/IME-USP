/*
 * Nome: Vítor Kei Taira Tamada
 * NUSP: 8516250
 * Lista 3 - Exercício 10
 * Algoritmos em Grafos
 */

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

/* Protótipos */
static link   NEWnode           (vertex, link);
       UGraph UGRAPHinit        (int);
       void   UGRAPHinsertEdge  (UGraph, vertex, vertex);
       void   UGRAPHshow        (UGraph);
       UGraph UGRAPHrand        (vertex, int);
       int    UGRAPHcc          (UGraph, int*);
static void   dfsRcc            (UGraph, int*, vertex, int);
       int*   ccVertexCounter   (UGraph, int*)

/* Cria um novo nó para a lista de adjacências */
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

/*
 * id == número de componentes conexas
 * cc[v] == id da componente conexo que ao qual o vértice v pertence
 */
int UGRAPHcc (UGraph UG, int *cc)
{
  vertex v;
  int id = 0;
  for (v = 0; v < UG->V; v++) cc[v] = -1;
  for (v = 0; v < UG->V; v++)
    if (cc[v] == -1)
      dfsRcc (UG, cc, v, id++);
  
  return (id);
}

/*
 * Atribui o número id a todos os vértices que
 * pertencem a mesma componente conexa que v
 */
static void dfsRcc (UGraph UG, int *cc, vertex v, int id)
{
  link a;
  cc[v] = id;
  for (a = UG->adj[v]; a != NULL; a = a->next)
    if (cc[a->w] == -1)
      dfsRcc (UG, cc, a->w, id);
}

/*
 * Conta o número de componentes conexas
 * com cada número número de vértices
 */
int* ccVertexCounter (UGraph UG, int *cc, int ccCount)
{
  int *ccVCount; /* ccVCount[i] == número vértices da componente conexa de id == i */
  vertex v;
  
  ccVCount = malloc (ccCount * sizeof (int));
  
  /* Utlizar busca em profundidade para encontrar o número de vértices da componente conexa */
}

int main (int argc, char *argv[])
{
  UGraph UG;
  int *cc, V, E, ccCount;
  
  srand (time (NULL));
  
  V = atoi (argv[1]);
  E = atoi (argv[2]);
  UG = UGRAPHrand (V, E);
  cc = malloc (V * sizeof (int));
  
  ccCount = UGRAPHcc (UG, cc);
  UGRAPHshow (UG);
  printf ("ccCount = %d\n", ccCount);
  return 0;
}
