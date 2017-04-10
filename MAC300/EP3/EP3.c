/*
    MAC 300 - Métodos Numéricos de Álgebra Linear

    Nome: Vítor Kei Taira Tamada
    NUSP: 8516250
    Exercício-programa 3 - Matrizes ortogonais e o problema de quadrados mínimos

    Comando de compilação:
        gcc -Wall -o EP3 EP3.c -lm

    Notas:
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/***********************/
/* Funções do programa */
/***********************/
int backrow (int, double**, double**);
void aux_values (int, int, double**, double*, double*);
void QA (int, int, int, double**, double*, double**);
void QR_decomposition (int, double**, double**, double*, double*);

int main ()
{
  /* Declaração de variáveis da função */
  FILE *file;
  char filePath[100];
  int i, j, n, suc, aux;
  double tal, gama, **A, **b, **x, num;

  /* Inicialização de variáveis */
  tal = 0;
  gama = 0;
  suc = 0;

  /* Recebe um arquivo */
  printf ("\nInsira o nome do arquivo que contém a matriz:\n");
  scanf ("%s", filePath);

  /* Verifica se o arquivo inserido existe e/ou foi inserido com sucesso */
  file = fopen (filePath, "r");
  if (file == NULL)
  {
      printf ("\n***** Erro ao abrir o arquivo *****\n\n");
      exit (-1);
  }

  /* Recebe a dimensão da matriz e verifica se é um valor válido */
  fscanf (file, "%d", &n);
  if (n <= 0)
  {
      printf ("\n***** Dimensão de matriz inválida *****\n\n");
      exit (-1);
  }

  /* Aloca a matriz A */
  A = malloc (n * sizeof(double));
  for (i = 0; i < n; i++)
    A[i] = malloc (n * sizeof (double));
  for (aux = 0; aux < n * n; aux++)
  {
      fscanf (file, "%d", &i);
      fscanf (file, "%d", &j);
      fscanf (file, "%lf", &num);
      A[i][j] = num;
  }

  /* Aloca uma matriz b de tamanho nx1 (um vetor de tamanho n) */
  b = malloc (n * sizeof (double));
  for (i = 0; i < n; i++)
    b[i] = malloc (sizeof (double));
  for (aux = 0; aux < n; aux++)
  {
      fscanf (file, "%d", &i);
      fscanf (file, "%lf", &num);
      *b[i] = num;
  }

  fclose (file);

  /* Aloca um vetor x. Ele é da forma de uma matriz nx1 */
  x = malloc (n * sizeof (double));
  for (i = 0; i < n; i++)
    x[i] = malloc (sizeof (double));
  for (i = 0; i < n; i++)
    x[i][0] = b[i][0];

  /* Com a decomposição QR por reflexão, tem-se QRx = b */
  QR_decomposition (n, A, b, &tal, &gama);

  /* QRx = b       */
  /* Rx = c        */
  /* Qc = b        */
  /* Q é ortogonal */
  /* c = (Q^T)*b   */
  for (i = 0; i < n - 1; i++)
    QA (n, 1, i, b, &gama, x);

  for (i = 0; i < n; i++)
    printf ("x[%d] = %f\n", i, b[i][0]);

  /* R armazenado em A     */
  /* c armazenado em b     */
  /* Rx = c                */
  /* Substituição por trás */
  /* (back substitution)   */
  suc = backrow (n, A, b);
  if (suc == -1)
  {
    printf ("***** R é singular ******\n");
    exit (-1);
  }
/*
  else
    for (i = 0; i < n; i++)
      printf ("x[%d] = %f\n", i, b[i][0]);
*/

  /* Liberação da memória */
  free (x);
  free (b);
  for (i = 0; i < n; i++)
    free(A[i]);
  free(A);

  return 0;
}

/* Cálculo de Ax=b sendo A uma matriz triangular superior */
/* Resultado armazenado em b                              */
int backrow (int n, double **A, double **b)
{
  /* Declaração de variáveis da função */
  int i, j, suc;
  /* suc = sucesso */

  for (i = n - 1; i > -1 && suc == 0; i--)
  {
    for (j = n - 1; j > i; j--)
      b[i][0] -= A[i][j] * b[j][0];

    if (A[i][i] == 0)
      suc = -1;
    else
      b[i][0] /= A[i][i];
  }

  return suc;
}

/* Algoritmo que calcula o valor de tal, gama e u tal que */
/*  Q = (I - gama * u * u^T) é um refletor para o qual  */
/*  Qx = [-r, 0, ..., 0]^T                              */
/*  O vetor u é salvo no vetor x                        */
void aux_values (int n, int k, double **x, double *tal, double *gama)
{
  /* Declaração de variáveis da função */
  int i;
  double beta, sum;

  /* Inicialização de variável */
  sum = 0;
  beta = 0;

  /* beta <- max(1<=i<=n) |x_i| */
  for (i = 0; i < n; i++)
  {
    if (beta < fabs (x[i][0]))
      beta = fabs (x[i][0]);
  }

  if (beta == 0)
    gama = 0;

  else
  {
    /* x[1:n] <- x[1:n] / beta */
    for (i = 0; i < n; i++)
      x[i][0] = x[i][0] / beta;

    /* tal <- sqrt (x[1]^2 + ... + x[n]^2) */
    for (i = 0; i < n; i++)
      sum = sum + x[i][0] * x[i][0];
    *tal = sqrt(sum);

    /* if (x[1] < 0) tal <- -tal */
    if (x[k] < 0)
      *tal = -*tal;

    /* x[1] <- tal + x[1] */
    x[k][0] = x[k][0] + *tal;

    /* gama <- x[1] / tal */
    *gama = x[k][0] / *tal;

    /* x[2:n] <- x[2:n] / x[1] */
    for (i = k + 1; i < n; i++)
      x[i][0] = x[i][0] / x[k][0];

    /* x[1] <- 1 */
    x[k][0] = 1;

    /* tal <- tal*beta */
    *tal = *tal * beta;
  }
}

/* Função auxiliar para a decomposição QR                      */
/* 3.2.40 do livro Fundamentals of Matrix Computations, 2nd ed */
void QA (int n, int m, int k, double **A, double *gama, double **u)
{
  /* Declaração de variáveis da função */
  int i, j, sum;
  double *v;

  sum = 0;

  v = malloc (n * sizeof (double));

  /* v^T <- gama * u^T */
  for (i = k; i < n; i++)
    v[i] = *gama * u[i][0];

  /* v^T <- v^T * B */
  for (j = k; j < m; j++)
  {
    for (i = k; i < n; i++)
      sum = sum + v[i] * A[i][j];

    v[i] = sum;
  }

  /* B <- B - u*v^T */
  for (i = k; i < n; i++)
    for (j = k; j < m; j++)
      A[i][j] = A[i][j] - u[i][0] * v[j];
}

/* Decomposição QR da matriz A                                */
/* A matriz triangular superior R é armazenada em A           */
/*  enquanto a matriz Q é guardada em u, tal e gama na forma  */
/*  Q = (I - gama * u * u^T)                                  */
/* 3.2.45 do livro Fundamentals of Matrix Computations 2nd ed */
void QR_decomposition (int n, double **A, double **u, double *tal, double *gama)
{
  /* Declaração de variáveis da função */
  int i, k;

  /* A matriz triangular superior R de A = Q*R é armazenada em A */
  for (k = 0; k < n - 1; k++)
  {
    /* Determinação do vetor u e dos valores auxiliares tal e gama */
    aux_values (n, k, u, tal, gama);

    /* Armazenamento do vetor u na coluna A[k] */
    for (i = k; i < n; i++)
      A[i][k] = u[i][0];

    /* a[k:n][k+1:n] <- (Q_k) * a[k:n][k+1:n] */
    QA (n, n, k, A, gama, u);

    /* A[k][k] <- -tal_k */
    /* tal se altera a cada iteração */
    A[k][k] = -*tal;
  }

  /* gama_n <- a[n][n] */
  *gama = A[n - 1][n - 1];
}
