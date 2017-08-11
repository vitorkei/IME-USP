/*
 * Nome: Vítor Kei Taira Tamada
 * NUSP: 8516250
 */

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
  int i, j;
  int** mat = (int**) malloc (R * sizeof(int*));
  for (i = 0; i < R; i++)
    mat[i] = (int*) malloc (C * sizeof(int));
  
  for (i = 0; i < R; i++)
    for (j = 0; j < C; j++)
      mat[i][j] = E;
  
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
    printf ("%d(%c): ", v, v+97);
    for (w = 0; w < G->V; w++)
      if (G->adj[v][w] == 1)
        printf ("%d(%c) ", w, w+97);
    printf("\n");
  }
}

int main()
{
  int i, j, sLen;
  Vertex v, w;
  char s[100];
  Graph G = GRAPHinit(26); /* Número de caracteres no alfabeto, desconsiderando ç e acentuados */
  
  printf("\nInsira o número de caracteres que serão lidos do texto inserido: ");
  scanf("%d", &sLen);
  
  printf("\nInsira uma string de comprimento mínimo de tamanho sLen = %d e de comprimento máximo 100 com apenas letras a-z e A-Z:\n", sLen);
  scanf("%s", s);
  
  for (i = 0; i < sLen - 1; i++)
  {
    j = 1; /* Auxiliar */
    
    if      (s[i] > 64 && s[i] < 91)  s[i] += 32; /* Se o caracter for A-Z, converte-o para a-z */
    else if (s[i] < 97 || s[i] > 122) continue;   /* Se o caracter não for uma letra a-z, pula para o próximo caracter do texto */
    
    /* A esta altura, o caracter lido deve ser uma letra a-z */
    
    /* Enquanto não for A-Z nem a-z */
    while (i+j < sLen)
      if(!((s[i+j] > 64 && s[i+j] < 91) || (s[i+j] > 96 && s[i+j] < 123)))
        j++; /* Procura o próximo caracter a-z ou A-Z */
      else break;
    
    if (s[i+j] > 64 && s[i+j] < 91) s[i+j] += 32; /* Converte uma letra A-Z em a-z */
    
    /* A esta altura, s[i] e s[i+j] são duas letras a-z, independente de quais caracteres especiais haja entre eles */
    
    /* Para facilitar o entendimento */
    v = s[i] - 97;
    w = s[i+j] - 97;
    
    GRAPHinsertArc(G, v, w);
  }
  
  printf("\n");
  GRAPHshow(G);
  
  return 0;
}

