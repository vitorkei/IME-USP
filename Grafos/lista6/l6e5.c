/*
 * Nome: Vítor Kei Taira Tamada
 * NUSP: 8516250
 * Lista 6 - Exercício 5
 * Algoritmos em Grafos
 */

#include <stdio.h>
#include <stdlib.h>
#define vertex int
#define greater(i, j) (prty[pq[i]] > prty[pq[j]] ? 1 : 0)

/* Fila de prioridade */
/* Algoritmo do site
   https://www.ime.usp.br/~pf/algoritmos_para_grafos/aulas/solutions/heap.html */
static vertex *pq;
static int N;
static int *qp;

/* Protótipos da fila de prioridade */
       void   PQinit  (int);
       int    PQempty ();
       void   PQinsert(vertex, int[]);
       vertex PQdelMin(int[]);
       void   PQdec   (vertex, int[]);
static void   exch    (int, int);
static void   fixUp   (int, int[]);
static void   fixDown (int, int[]);
       void   PQfree  ();

void PQinit(int maxN)
{
  pq = malloc((maxN+1) * sizeof(vertex));
  qp = malloc(maxN * sizeof(int));
  N = 0;
}

int PQempty() { return N == 0; }

void PQinsert(vertex v, int prty[])
{
  qp[v] = ++N;
  pq[N] = v;
  fixUp(N, prty);
}

vertex PQdelMin(int prty[])
{
  exch(1, N);
  --N;
  fixDown(1, prty);
  return pq[N+1];
}

void PQdec(vertex w, int prty[])
{
  fixUp(qp[w], prty);
}

static void exch(int i, int j)
{
  vertex t;
  
  t = pq[i];
  pq[i] = pq[j];
  pq[j] = t;
  
  qp[pq[i]] = i;
  qp[pq[j]] = j;
}

static void fixUp(int k, int prty[])
{
  while(k > 1 && greater(k/2, k))
  {
    exch(k/2, k);
    k = k / 2;
  }
}

static void fixDown(int k, int prty[])
{
  int j;
  while(2*k <= N)
  {
    j = 2 * k;
    if(j < N && greater(j, j+1)) j++;
    if(!greater(k, j)) break;
    exch(k, j);
    k = j;
  }
}

void PQfree()
{
  free(pq);
  free(qp);
}

/*********** GRAFOS ***********/

/* Estrutura */
typedef struct node *link;
struct node{
  vertex w;
  link next;
  int cv; /* custo do vértice */
};
struct graph{
  int V; /* Vertices */
  int A; /* Arestas  */
  link *adj;
  int *cost;
};
typedef struct graph *Graph;

/* Protótipos */
static link  NEWnode       (vertex, link);
       Graph GRAPHinit     (int);
       void  GRAPHinsertArc(Graph, vertex, vertex);
       void  GRAPHshow     (Graph);
       Graph GRAPHrand     (vertex, int);

/* Cria um novo nó para a lista de adjacências */
static link NEWnode(vertex w, link next){
  link a = malloc(sizeof(struct node));
  a->w = w;
  a->next = next;
  return a;
}

Graph GRAPHinit(int V)
{
  vertex v;
  
  Graph G = malloc(sizeof *G);
  
  G->V = V;
  G->A = 0;
  G->adj = malloc(V * sizeof(link));
  G->cost = malloc(V * sizeof(int));
  for(v = 0; v < V; v++)
  {
    G->adj[v] = NULL;
    G->cost[v] = rand() % V;
  }
  
  return G;
}

void GRAPHinsertArc(Graph G, vertex v, vertex w)
{
  link a;
  
  for(a = G->adj[v]; a != NULL; a = a->next)
    if(a->w == w)
      return;
  
  G->adj[v] = NEWnode(w, G->adj[v]);
  G->A++;
}

void GRAPHshow(Graph G)
{
  vertex v;
  link p;
  
  printf("Arcos (custos do vértice):\n");
  for(v = 0; v < G->V; v++)
  {
    printf("%d (%d): ", v, G->cost[v]);
    for(p = G->adj[v]; p != NULL; p = p->next)
      printf("%d ", p->w);
    printf("\n");
  }
  printf("G->A = %d\n\n", G->A);
}

Graph GRAPHrand(vertex V, int E)
{
  vertex v, w;
  double p =(double) E /(V *(V-1));
  Graph G = GRAPHinit(V);
  
  for(v = 0; v < V; v++)
    for(w = 0; w < V; w++)
      if(v != w)
        if(rand() < p * (RAND_MAX + 1.0))
          GRAPHinsertArc(G, v, w);
  
  return(G);
}

/*
 * Algoritmo retirado do site https://www.ime.usp.br/~pf/algoritmos_para_grafos/aulas/dijkstra.html (GRAPHspt3())
 * O maior custo que um vértice pode ter é G->V - 1, então
 * o maior custo que um caminho pode ter é G->V*(G->V - 1),
 * que é um caminho que passe por todos os vértices e todos têm
 * custo máximo. Logo, INF = (G->V)^2 para simplificar.
*/
void GRAPHspt(Graph G, vertex s, vertex *parent, int *dist)
{
  int INF = G->V * G->V, cost;
  vertex v, u, w;
  link p;
  vertex *hook = malloc(G->V * sizeof(vertex));
  
  /* Inicialização */
  for(v = 0; v < G->V; v++)
  {
    parent[v] = -1;
    dist[v] = INF;
  }
  parent[s] = s;
  dist[s] = 0;
  for(p = G->adj[s]; p != NULL; p = p->next)
  {
    dist[p->w] = G->cost[p->w];
    hook[p->w] = s;
  }
  PQinit(G->V);
  for(v = 0; v < G->V; v++)
    if(v != s) PQinsert(v, dist);
  
  while(!PQempty())
  {
    u = PQdelMin(dist);
    if(dist[u] == INF) break;
    parent[u] = hook[u];
    
    /* Atualização de dist[] */
    for(p = G->adj[u]; p != NULL; p = p->next)
    {
      w = p->w;
      cost = G->cost[w];
      if(!(dist[u] + cost >= dist[w]))
      {
        dist[w] = dist[u] + cost; /* Relaxa u-w */
        PQdec(w, dist);
        hook[w] = u;
      }
    }
  }
  
  for(v = 0; v < G->V; v++)
    if(dist[v] != INF)
      dist[v] += G->cost[s];
  
  PQfree();
  free(hook);
}

int main(int argc, char *argv[])
{
  int V, E, *dist, INF;
  vertex *parent, v;
  Graph G;
  
  srand(time(NULL));
  
  V = atoi(argv[1]);
  E = atoi(argv[2]);
  INF = V * V;
  
  dist = malloc(V * sizeof(int));
  parent = malloc(V * sizeof(vertex));
  
  G = GRAPHrand(V, E);
  GRAPHspt(G, 0, parent, dist);  
  GRAPHshow(G);
  
  printf("dist[]:\n");
  for(v = 0; v < V; v++)
  {
    printf("dist[%d] = ", v);
    if(dist[v] == INF) printf("INF\n");
    else printf("%d\n", dist[v]);
  }
  free(dist);
  free(parent);
  
  return 0;
}
