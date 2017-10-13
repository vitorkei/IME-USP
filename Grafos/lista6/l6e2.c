#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define vertex int
#define greater(i, j) (prty[pq[i]] > prty[pq[j]] ? 0 : 1)

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


/* Estrutura */
typedef struct node *link;
struct node{
  vertex w;
  link next;
  int cst; /* custo do vértice */
};
struct graph{
  int V; /* Vertices */
  int A; /* Arestas  */
  link *adj;
};
typedef struct graph *Graph;

/* Protótipos */
static link  NEWnode       (vertex, link, int);
       Graph GRAPHinit     (int);
       void  GRAPHinsertArc(Graph, vertex, vertex);
       void  GRAPHshow     (Graph);
       Graph GRAPHrand     (vertex, int);

/* Cria um novo nó para a lista de adjacências */
static link NEWnode(vertex w, link next, int cst){
  link a = malloc(sizeof(struct node));
  a->w = w;
  a->next = next;
  a->cst = cst;
  return a;
}

Graph GRAPHinit(int V)
{
  vertex v;
  
  Graph G = malloc(sizeof *G);
  
  G->V = V;
  G->A = 0;
  G->adj = malloc(V * sizeof(link));
  for(v = 0; v < V; v++)
    G->adj[v] = NULL;
  
  return G;
}

void GRAPHinsertArc(Graph G, vertex v, vertex w)
{
  link a;
  int cst;
  
  for(a = G->adj[v]; a != NULL; a = a->next)
    if(a->w == w)
      return;
  
  cst = rand() % G->V;
  
  G->adj[v] = NEWnode(w, G->adj[v], cst);
  G->A++;
}

void GRAPHshow(Graph G)
{
  vertex v;
  link p;
  
  printf("Arcos (custos do arco):\n");
  for(v = 0; v < G->V; v++)
  {
    printf("%d: ", v);
    for(p = G->adj[v]; p != NULL; p = p->next)
      printf("%d (%d), ", p->w, p->cst);
    printf("\n");
  }
  printf("G->A = %d\n\n", G->A);
}

Graph GRAPHrand(vertex V, int E)
{
  vertex v, w;
  double p = (double) E / (V * (V-1));
  Graph G = GRAPHinit(V);
  
  for(v = 0; v < V; v++)
    for(w = 0; w < V; w++)
      if(v != w)
        if(rand() < p * (RAND_MAX + 1.0))
          GRAPHinsertArc(G, v, w);
  
  return(G);
}

int GRAPHsptD( Graph G, vertex s, vertex *parent, int *dist, vertex *T)
{
   vertex *hook = malloc( G->V * sizeof (vertex));
   vertex v, y, z, w;
   link a;
   int mindist;
   int cst;
   int INF = G->V * G->V;
   
   // inicialização:
   for (v = 0; v < G->V; v++)
      parent[v] = -1, dist[v] = INF;
   parent[s] = s, dist[s] = 0, hook[s] = s;
   for (a = G->adj[s]; a != NULL; a = a->next) {
      dist[a->w] = a->cst;
      hook[a->w] = s;
   }

   while (1) {
      // cálculo de y:
      mindist = INF;
      for(z = 0; z < G->V; z++)
         if (parent[z] == -1 && dist[z] < mindist)
            mindist = dist[z], y = z;
      if (mindist == INF) break;
      if (T[y] == 1) return dist[y];
      parent[y] = hook[y];
      // atualização de dist[]:
      for (a = G->adj[y]; a != NULL; a = a->next) {
         w = a->w;
         cst = a->cst;
         if (!(dist[y] + cst >= dist[w])) {
            dist[w] = dist[y] + cst; // relaxa y-w
            hook[w] = y;
         }
      }
   }
   free( hook);
   return INF;
}

int GRAPHdistST(Graph G, vertex *S, vertex *T, vertex *parent, vertex *dist)
{
  vertex s;
  int minDist = G->V * G->V, aux;
  
  for (s = 0; s < G->V; s++)
  {
    if (S[s] == 1)
    {
      if (T[s] == 1) return 0;
      printf("s = %d\n", s);
      aux = GRAPHsptD(G, s, parent, dist, T);
      if (minDist > aux) minDist = aux;
      printf("minDist = %d\n\n", minDist);
      if (minDist == 0) return minDist;
    }
  }
  return minDist;
}


void printMinDist(Graph G, vertex *S, vertex *T, int minDist)
{
  vertex v;
  for (v = 0; v < G->V; v++)
    printf("S[%d] = %d || T[%d] = %d\n", v, S[v], v, T[v]);
  printf("minDist = ");
  if (minDist == G->V * G->V) printf ("INF\n\n");
  else printf("%d\n\n", minDist);
}

int main(int argc, char *argv[])
{
  int V, E, *dist, INF, minDist, s, t;
  vertex *parent, v, *S, *T;
  Graph G;
  time_t t0;
  
  srand(time(&t0));
  
  V = atoi(argv[1]);
  E = atoi(argv[2]);
  
  dist = malloc(V * sizeof(int));
  parent = malloc(V * sizeof(vertex));
  S = malloc(V * sizeof(vertex));
  T = malloc(V * sizeof(vertex));
  
  for (v = 0; v < V; v++)
  {
    s = rand() % 2;
    if (s == 0) S[v] = 0;
    else if (s == 1) S[v] = 1;
    
    do{
    t = rand() % 2;
    if (t == 0) T[v] = 0;
    else if (t == 1) T[v] = 1;
    } while(t == s);
  }
  
  G = GRAPHrand(V, E);
  minDist = GRAPHdistST(G, S, T, parent, dist);
  printMinDist(G, S, T, minDist);
  GRAPHshow(G);
  
  free(dist);
  free(parent);
  
  return 0;
}
