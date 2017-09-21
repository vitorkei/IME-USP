/*
 * Nome: Vítor Kei Taira Tamada
 * NUSP: 8516250
 * Lista 5 - Exercício 7
 * Algoritmos em Grafos
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define vertex int
#define UGraph Graph
#define maxV 1000

/*********** FILAS ***********/
/* 
 * Implementação de fila em um vetor segundo o site
 * https://www.ime.usp.br/~pf/algoritmos/aulas/fila.html
 */

int *queue, ini, end;

/* Protótipos */
void   QUEUEinit  (int);
int    QUEUEempty ();
void   QUEUEput   ();
vertex QUEUEget   ();
void   QUEUEfree  ();

void QUEUEinit (int V)
{
  queue = malloc (V * sizeof(int));
  ini = end = 0;
}

int    QUEUEempty ()       { return ini >= end; }

void   QUEUEput (vertex v) { queue[end++] = v; }

vertex QUEUEget ()         { return queue[ini++]; }

void   QUEUEfree ()        { free (queue); }

/*********** GRAFOS ***********/

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
       void   UGRAPHminPaths      (UGraph, vertex, int*, vertex*);

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

void UGRAPHminPaths (UGraph UG, vertex s, int *dist, vertex *parent)
{
  const int INF = UG->V;
  vertex v;
  
  for (v = 0; v < UG->V; v++)
  {
    parent[v] = -1;
    dist[v] = INF;
  }
  QUEUEinit (UG->V);
  dist[s] = 0;
  parent[s] = s;
  QUEUEput (s);
  
  while (!QUEUEempty ())
  {
    link a;
    v = QUEUEget ();
    for (a = UG->adj[v]; a != NULL; a = a->next)
    {
      vertex w = a->w;
      if (parent[w] == -1)
      {
        parent[w] = v;
        dist[w] = dist[v] + 1;
        QUEUEput (w);
      }
    }
  }
}

int main (int argc, char *argv[])
{
  /*
   * meanDistUG é a distância média entre dois vértices em um dado grafo
   * meandistAllUG é a distância méida entre dois vértices de todos os grafos analisados
   */
  int i, j, k, V, E, *dist, disconnected, count;
  int arrayV[5] = {10, 20, 30, 40, 50};
  double arrayE[8] = {.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0};
  double meanDistUG, meanDistAllUG = 0;
  vertex *parent, s, v;
  UGraph UG;
  
  srand (time (NULL));
  
  for (i = 0; i < 5; i++)
  {
    for (j = 0; j < 8; j++)
    {
      V = arrayV[i];
      E = arrayV[i] * arrayE[j];
      count = 0;
      
      printf("V = %d || E = %d\n", V, E);
      
      UG = UGRAPHrand (V, E);
      dist     = malloc ((UG->V) * sizeof (int));
      parent   = malloc ((UG->V) * sizeof (vertex));
      
      disconnected = 0;
      for (s = 0; s < UG->V; s++)
      {
        meanDistUG = 0;
        UGRAPHminPaths (UG, s, dist, parent);
        
        for (v = 0; v < UG->V && !disconnected; v++)
          if (dist[v] == UG->V)
          {
            disconnected = 1;
            break;
          }
        
        if (!disconnected)
        {
          printf ("s = %d\n", s);
          for (v = 0; v < UG->V; v++)
          {
            meanDistUG += (double) dist[v];
            printf ("dist[%d] = %d\n", v, dist[v]);
          }
          meanDistUG /= (UG->V - 1);
          
          meanDistAllUG += meanDistUG;
          count++;
        }
        else
        {
          printf ("Distância média = INF\n\n");
          meanDistAllUG = -1;
          break;
        }
      }
      if (meanDistAllUG != -1)
      {
        meanDistAllUG /= count;
        printf ("..Distância média = %f\n\n", meanDistAllUG);
      }
      
      free (dist);
      free (parent);
    }
  }
  
  return 0;
}
