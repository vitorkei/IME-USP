/*
 * Nome: Vítor Kei Taira Tamada
 * NUSP: 8516250
 * Lista 4 - Exercício 8
 * Algoritmos em Grafos
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define vertex int
#define UGraph Graph
#define maxV 1000

int low[maxV];
int pre[maxV];
int cnt;
vertex parent[maxV];


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
       UGraph UGRAPHinit          (int);
       void   UGRAPHinsertEdge    (UGraph, vertex, vertex);
       void   UGRAPHshow          (UGraph);
       UGraph UGRAPHrand          (vertex, int);
       int    UGRAPHcc            (UGraph, int*);
static void   dfsRcc              (UGraph, vertex, int*, int);
       void   UGRAPHbridges       (UGraph);
       void   bridgesR            (UGraph, vertex);

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
  printf ("Arestas:\n");
  for (v = 0; v < UG->V; v++)
  {
    printf ("%d: ", v);
    for (p = UG->adj[v]; p != NULL; p = p->next)
      printf ("%d ", p->w);
    printf ("\n");
  }
  printf("UG->E = %d\n\n", UG->E);
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
 * id é o número de componentes conexas que o grafo tem
 * cc[v] é a id da componente conexa ao qual o vértice v pertence
 *  logo, cc[v] == cc[u] se e somente se v e u pertencem a mesma
 *  componente conexa
 */
int UGRAPHcc (UGraph UG, int *cc)
{
  vertex v;
  int id = 0;
  for (v = 0; v < UG->V; v++) cc[v] = -1;
  for (v = 0; v < UG->V; v++)
    if (cc[v] == -1)
      dfsRcc (UG, v, cc, id++);
  
  return id;
}

/*
 * Encontra o número de componentes conexas por meio
 * de uma busca em profundidade
 */
void dfsRcc (UGraph UG, vertex v, int *cc, int id)
{
  link a;
  cc[v] = id;
  for (a = UG->adj[v]; a != NULL; a = a->next)
    if (cc[a->w] == -1)
      dfsRcc (UG, a->w, cc, id);
}

/********************/

void UGRAPHbridges (UGraph UG)
{
  vertex v;
  
  for (v = 0; v < UG->V; v++)
  {
    pre[v] = -1;
    parent[v] = -1;
  }
  
  cnt = 0;
  for (v = 0; v < UG->V; v++)
    if (pre[v] == -1)
    {
      parent[v] = v;
      bridgesR (UG, v);
    }
  
  printf ("Pontes:\n");
  for (v = 0; v < UG->V; v++)
    if (low[v] == pre[v] && parent[v] != v)
      printf ("..%d-%d\n", parent[v], v);
}

void bridgesR (UGraph UG, vertex v)
{
  link p;
  int min;
  vertex w;
  pre[v] = cnt++;
  
  min = pre[v];
  
  for (p = UG->adj[v]; p != NULL; p = p->next)
  {
    w = p->w;
    if (pre[w] == -1)
    {
      parent[w] = v;
      bridgesR (UG, w);
      
      if (low[w] < min) min = low[w];
    }
    else
    {
      if (pre[w] < min && w != parent[v]) min = pre[w];
    }
  }
  
  low[v] = min;
}

/********************/

int main (int argc, char *argv[])
{
  int *cc, V, E, id;
  UGraph UG;
  
  V = atoi (argv[1]);
  E = atoi (argv[2]);
  
  srand (time (NULL));
  
  cc = malloc (V * sizeof (int));
  UG = UGRAPHrand (V, E);
  id = UGRAPHcc (UG, cc);
  
  UGRAPHshow (UG);
  
  UGRAPHbridges (UG);
  
  return 0;
}
