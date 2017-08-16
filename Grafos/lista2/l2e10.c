#include <stdlib.h>
#include <stdio.h>
#define Vertex int

static int visit[1000]; /* Considerando um grafo de, no máximo, 1000 vértices */

/* Estrutura */
struct graph
{
  int V;     /* Número de vértices   */
  int E;     /* Número de arestas    */
  int **adj; /* Matriz de adjacência */
};
typedef struct graph *Graph;

void GRAPHerror (Vertex v, Vertex w)
{
  printf ("G->adj[%d][%d] == 0, mas G->adj[%d][%d] == 1\n", v, w, w, v);
  exit (1);
}

/*
 * Instancia uma matrix de R linhas, C colunas
 *   e elementos inicializados com o valor E
*/
int** MATRIXinit (int R, int C, int E)
{
  int i, j;
  int** mat = (int**) malloc (R * sizeof(int*));
  for (i = 0; i < R; i++)
    mat[i] = (int*) malloc (C * sizeof(int));
  
  for (i = 0; i < R; i++)
    for (j = 0; j < C; j++)
      mat[i][j] = E;
  
  return mat;
}

/* Inicializa um grafo não-dirigido com V vértices e sem arestas */
Graph GRAPHinit (int V)
{
  Graph G = malloc (sizeof * G);
  G->V = V;
  G->E = 0;
  G->adj = MATRIXinit(V, V, 0);
  return (G);
}

/* Insere aresta entre vértices v e w */
void GRAPHinsertEdge (Graph G, Vertex v, Vertex w)
{
  if (v != w)
    if (G->adj[v][w] == 0)
    {
      if (G->adj[w][v] == 0)
      {
        G->adj[v][w] = 1; /* Desta forma, é possível ver que existe aresta */
        G->adj[w][v] = 1; /* entre v e w a partir de qualquer um dos vértices */
        G->E++;
      }
      else /* Para detectar erros */
        GRAPHerror (v, w);
    }
}

/* Remove aresta entre vértices v e w */
void GRAPHremoveEdge (Graph G, Vertex v, Vertex w)
{
  if (G->adj[v][w] == 1)
  {
    if (G->adj[w][v] == 1)
    {
      G->adj[v][w] = 0;
      G->adj[w][v] = 0;
      G->E--;
    }
    else /* Para detectar erros */
      GRAPHerror (v, w);
  }
}

/* Imprime o grafo */
void GRAPHshow (Graph G)
{
  Vertex v, w;
  for (v = 0; v < G->V; v++)
  {
    printf ("%d: ", v);
    for (w = 0; w < G->V; w++)
      if (G->adj[v][w] == 1)
        printf ("%d ", w);
        
    printf ("\n");
  }
  printf("%d arestas\n", G->E);
}

/*
 * Gera um grafo não-dirigido aleatório
 * com V vértices e número esperado de 
 * E arestas
*/
Graph GRAPHrand (Vertex V, int E)
{
  time_t t;
  Vertex v, w;
  double p = (double) E / (V * (V-1));
  Graph G = GRAPHinit (V);
  
  srand (time (&t));
  
  for (v = 0; v < V; v++)
    for (w = 0; w < V; w++)
      if (v != w)
        if (rand() < p * (RAND_MAX + 1.0))
          GRAPHinsertEdge(G, v, w);
  
  return (G);
}

/* Função auxiliar para saber se dois vértices são conexos */
void reachR (Graph G, Vertex v)
{
  Vertex w;
  visit[v] = 1;
  for (w = 0; w < G->V; w++)
    if (G->adj[v][w] == 1 && visit[w] == 0)
      reachR (G, w);
}

/* Função que retorna se dois vértices são conexos */
int GRAPHreach (Graph G, Vertex s, Vertex t)
{
  Vertex v;
  for (v = 0; v < G->V; v++)
    visit[v] = 0;
  reachR (G, s);
  if (visit[t] == 0) return 0;
  return 1;
}

int main ()
{
  Graph G;
  Vertex V, E, s, t;
  printf ("Insira o número de vértices: ");
  scanf ("%d", &V);
  
  printf ("Insira o número esperado de arestas: ");
  scanf ("%d", &E);
  
  G = GRAPHrand (V, E);
  
  GRAPHshow (G);
  
  printf ("Insira dois vértices no intervalo [0, %d] para saber se são conexos: ", (G->V)-1);
  scanf ("%d %d", &s, &t);
  
  if (GRAPHreach(G, s, t)) printf ("Conexos\n");
  else                     printf ("Não conexos\n");
  
  return 0;
}
