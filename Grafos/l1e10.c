#include <stdio.h>
#include <stdlib.h>
#define Vertex int

struct graph{
  int V;     /* Número de vértices */
  int A;     /* Número de arcos */
  int **adj; /* matriz de adjacência */
};
typedef struct graph* Graph;

int** MATRIXinit(int R, int C, int E)
{
  printf("blub\n");
  int i, j;
  int** mat = (int**) malloc (R * sizeof(int*));
  for (i = 0; i < R; i++)
    mat[i] = (int*) malloc (C * sizeof(int));
  
  for (i = 0; i < R; i++)
    for (j = 0; j < C; j++)
      mat[i][j] = E;
  
  printf("bleb\n");
  return mat;
}

/* Inicializa */
Graph GRAPHinit (int V)
{
  Graph G = malloc (sizeof * G);
  G->V = V;
  G->A = 0;
  G->adj = MATRIXinit(V, V, 0);
  return (G);
}

/* Adiciona arco */
void GRAPHinsertArc (Graph G, Vertex v, Vertex w)
{
  if (v != w && G->adj[v][w] == 0)
  {
    G->adj[v][w] = 1;
    G->A++;
  }
}

/* Remove arco */
void GRAPHremoveArc (Graph G, Vertex v, Vertex w)
{
  if (G->adj[v][w] == 1)
  {
    G->adj[v][w] = 0;
    G->A--;
  }
}

/* Verifica se é fonte */
int GRAPHsource (Graph G, Vertex v)
{
  Vertex u;
  for (u = 0; u < G->V; u++)
    if (G->adj[u][v] == 1)
      return 0;
  return 1;
}

void GRAPHshow (Graph G)
{
  Vertex v, w;
  for (v = 0; v < G->V; v++)
  {
    printf ("%d: ", v);
    for (w = 0; w < G->V; w++)
      if (G->adj[v][w] == 1)
        printf ("%d ", w);
    printf("\n");
  }
}

int main()
{
  int i, sLen;
  char* s;
  Graph G = GRAPHinit(26); /* Número de caracteres no alfabeto, desconsiderando ç e acentuados */
  
  printf("Insira o número de caracteres que serão lidos do texto inserido: ");
  scanf("%d", &sLen);
  
  printf("Insira o texto que formará o grafo.\nSe o número de caracteres lidos for menor que o tamanho do texto, haverá erros.\nCaracteres maiúsculos não são diferenciados de minúsculos:\n");
  scanf("%s", s);
  
  for (i = 0; i < sLen; i++)
  {
    if (s[i] > 64 && s[i] < 91) s[i] += 32; /* Converte para minúsculo */
    if (s[i]
  }
  
  return 0;
}

/* TESTE
int main()
{
  Graph G = GRAPHinit(10);
  int test = 123456789;
  Vertex v, w;
  for (v = test % 10, w = (test / 10) % 10; test / 10 != 0; test = test / 10, v = test % 10, w = (test / 10) % 10)
  {
    printf("%d -> %d\n", w, v);
    GRAPHinsertArc (G, w, v);
    printf("%d\n", test);
  }
  
  GRAPHshow (G);
  
  return 0;
}
*/
