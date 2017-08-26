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
static link   NEWnode             (vertex, link);
       UGraph UGRAPHinit          (int);
       void   UGRAPHinsertEdge    (UGraph, vertex, vertex);
       void   UGRAPHshow          (UGraph);
       UGraph UGRAPHrand          (vertex, int);
       int    UGRAPHcc            (UGraph, int*, int*);
static void   dfsRcc              (UGraph, int*, vertex, int, int*);
       void   UGRAPHccVertexCount (int *ccVertexCount, int ccCount);

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
 * id == número de componentes conexas
 * cc[v] == id da componente conexo que ao qual o vértice v pertence
 */
int UGRAPHcc (UGraph UG, int *cc, int *ccVertexCount)
{
  vertex v;
  int id = 0;
  for (v = 0; v < UG->V; v++) cc[v] = -1;
  for (v = 0; v < UG->V; v++)
    if (cc[v] == -1)
      dfsRcc (UG, cc, v, id++, ccVertexCount);
  
  return (id);
}

/*
 * Atribui o número id a todos os vértices que
 * pertencem a mesma componente conexa que v
 */
static void dfsRcc (UGraph UG, int *cc, vertex v, int id, int *ccVertexCount)
{
  link a;
  cc[v] = id;
  ccVertexCount[id]++;
  for (a = UG->adj[v]; a != NULL; a = a->next)
    if (cc[a->w] == -1)
      dfsRcc (UG, cc, a->w, id, ccVertexCount);
}

/* Imprime o número de vértices em cada componente, ordenada pelos ids */
void UGRAPHccVertexCount (int *ccVertexCount, int ccCount)
{
  int i;
  printf ("Número de vértices em cada componente:\n");
  for (i = 0; i < ccCount; i++)
    printf("%d) %d\n", i, ccVertexCount[i]);
}

int main (int argc, char *argv[])
{
  UGraph UG;
  /*
   * cc[v] == id da componente conexa ao qual vértice v pertence
   * ccCount == número de componente conexas
   * ccVertexCount[k] == número de vértices da componente conexa de id == k
   * ccVCmean[k] == número médio de componentes com k vértices
   */
  int *cc, V, E, ccCount, *ccVertexCount, *ccVCmean, i, j, k, l;
  int arrayV[5] = {10, 20, 30, 40, 50};
  double arrayE[8] = {0.2, 0.5, 1.0, 2.0, 5.0, 10.0, 20.0, 30.0};
  double vertexMean;
  
  srand (time (NULL));
  
  cc = malloc (V * sizeof (int));
  /* Teste terá 50 vértices no máximo, logo, 50 cc no máximo. Desconsidera cc de 0 vértices */
  ccVertexCount = malloc (51 * sizeof (int));
  ccVCmean = malloc (51 * sizeof (int));
  
  /* arrayV tem 5 elementos */
  for (i = 0; i < 5; i++)
  {
    V = arrayV[i];
    /* arrayE tem 8 elementos */
    for (j = 0; j < 8; j++)
    {
      E = arrayE[j] * V;
      
      printf("(V = %d || E = %d)\n", V, E);
      
      /* Reinicia o vetor que conta as componentes conexas com k vértices */
      for (k = 0; k < 51; k++) ccVCmean[k] = 0;
      
      /* Roda 100 vezes e tira a média do número de componentes com cada número de vértices */
      for (k = 0; k < 100; k++)
      {
        /* Reinicia os vetores */
        for (l = 0; l < V; l++) cc[l] = -1;
        for (l = 0; l < 51; l++) ccVertexCount[l] = 0;
        
        UG = UGRAPHrand (V, E);
        ccCount = UGRAPHcc (UG, cc, ccVertexCount);
        
        /* e.g.: se uma componente conexa tem 5 vértices, ccVCmean[5]++ */
        for (l = 0; l < ccCount; l++)
          ccVCmean[ccVertexCount[l]]++;
      }
      
      /* Imprime o número médio de componentes conexas com cada número de vértices */
      /* Imprime apenas se tiver aparecido pelo menos uma componente conexa com k vértices */
      for (k = 1; k < 51; k++)
        if (ccVCmean[k] > 0)
        {
          vertexMean = (double) ccVCmean[k] / 100;
          printf ("..%d vértices: %.2f\n", k, vertexMean);
        }
    }
  }
  
  return 0;
}
