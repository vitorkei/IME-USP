/*
    MAC 300 - Métodos Numéricos de Álgebra Linear

    Nome: Vítor Kei Taira Tamada
    NUSP: 8516250
    Exercício-programa 2 - Métodos iterativos para sistemas lineares: Gradientes Conjugados

    Comando de compilação:
        gcc -Wall -o EP2 EP2.c -lm

    Notas:
        -O formato da entrada de dados é a mesma do EP1; inclusive, os testes foram feitos com os mesmos exemplos (tanto os gerados pelos códigos fornecidos quanto os menores criados por mim)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Struct para armazenar a matriz esparsa */
typedef struct SprsMat
{
    double num;
    int i, j;
    struct SprsMat *prox;
} SparseMatrix;

/**********************/
/* Funções auxiliares */
/**********************/

SparseMatrix* newCell (double, int, int);
SparseMatrix* MatEsp (int, double**);
double* MatxVec (int, SparseMatrix*, double*);
double VecxVec (int, double*, double*);
double* cxVec (int, double*, double);
int is_r_0 (int n, double*);

int main ()
{
    /* Declaração de variáveis */
    FILE *file;
    char filePath[100];
    int i, j, n, k, l, flag;
    SparseMatrix *MEA; /* MEA = Matriz Esparsa A */
    double **A, *r, *p, *x, *q, va, vd, u, alpha, beta, *aux, num;
    /*
        va = v_antes
        vd = v_depois
    */

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

    /* Cria a matriz A[n][n] */
    A = malloc (n * sizeof (double*));
    for (i = 0; i < n; i++)
        A[i] = malloc (n * sizeof (double));
    for (k = 0; k < n * n; k++)
    {
        fscanf (file, "%d", &i);
        fscanf (file, "%d", &j);
        fscanf (file, "%lf", &num);
        A[i][j] = num;
    }

    /* Cria o vetor "b[n]"                                                                          */
    /* Coloca-se o vetor b em r porque ele é usado apenas para encontrar o primeiro r               */
    /* Equivale a linha r <- r - Ax no algoritmo do livro Fundamentals of Linear Algebra (pag 577)  */
    /* Considerando x0 = 0, r <- r - Ax fica apenas r <- r                                          */
    r = malloc (n * sizeof (double));
    for (k = 0; k < n; k++)
    {
        fscanf (file, "%d", &i);
        fscanf (file, "%lf", &num);
        r[i] = num;
    }
    fclose (file);

    /* Criação dos outros vetores utilizados pelo algoritmo */
    p = malloc (n * sizeof (double));
    x = malloc (n * sizeof (double));
    q = malloc (n * sizeof (double));
    aux = malloc (n * sizeof (double));

    /* Vetor inicial x(0) é igual a 0 */
    for (i = 0; i < n; i++)
        x[i] = 0;

    /* p <- r */
    for (i = 0; i < n; i++)
        p[i] = r[i];

    /* v <- (r^T)*r */
    va = VecxVec (n, r, r);

    /* Transforma a matriz esparsa em uma lista ligada que contém apenas os elementos não-nulos e suas respectivas coordenadas na matriz */
    MEA = MatEsp (n, A);

    printf ("Seja l o número limite de iterações. Escolha um l:\n");
    scanf ("%d", &l);

    /* Flag que indica se houve convergência ou não */
    flag = 0;

    for (k = 0; flag == 0 && k < l; k++)
    {
        /* q = A*p                                              */
        /* Este algoritmo funciona apenas se a matriz inserida  */
        /*    estiver no formato de saída da função MatEsp      */
        q = MatxVec (n, MEA, p);

        /* u = (p^T)*q */
        u = VecxVec (n, p, q);

        alpha = va / u;

        /* x = x + a*p */
        aux = cxVec (n, p, alpha);  /* a*p         */
        for (i = 0; i < n; i++)     /* x = x + a*p */
            x[i] = x[i] + aux[i];

        /* r = r - a*q */
        aux = cxVec (n, q, alpha);  /* a*q          */
        for (i = 0; i < n; i++)     /* r = r - a*q  */
            r[i] = r[i] - aux[i];

        /* vd = (r^T)*r */
        vd = VecxVec (n, r, r);

        beta = vd / va;

        /* p = r + beta * p */
        aux = cxVec (n, p, beta);   /* beta*p           */
        for (i = 0; i < n; i++)     /* p = r + beta*p   */
            p[i] = r[i] + aux[i];

        va = vd;

        flag = is_r_0 (n, r);
    }

    if (flag == 1)
        printf ("Houve convergência em %d iterações.\n", l);
    else
        printf ("Não houve convergência em %d iterações.\n", l);

    printf ("Mostrar os valores do vetor r?\nDigite 1 para mostrar e qualquer outra coisa caso contrário: ");
    scanf ("%d", &i);
    if (i == 1)
        for (i = 0; i < n; i++)
            printf ("r[%d] = %f\n", i, r[i]);

    printf ("\nMostrar os valores do vetor x?\nDigite 1 para mostrar e qualquer outra coisa caso contrário: ");
    scanf ("%d", &i);
    if (i == 1)
        for (i = 0; i < n; i++)
            printf ("x[%d] = %f\n", i, x[i]);

    /* Liberação da memória */
    for (i = 0; i < n; i++)
        free (A[i]);

    free (A);
    free (r);
    free (p);
    free (q);
    free (x);
    free (aux);

    return 0;
}

/* Cria uma nova célula da lista ligada que formará a matriz esparsa */
SparseMatrix* newCell (double num, int i, int j)
{
    /* Cria a variável (célula nova) */
    SparseMatrix *newCell;

    /* Atribui os valores da célula (número e coordenadas da célula na matriz esparsa) */
    newCell = malloc (sizeof (double));
    newCell->num = num;
    newCell->i = i;
    newCell->j = j;
    newCell->prox = NULL;
    return newCell;
}

/* Cria a lista ligada referente a matriz esparsa A */
SparseMatrix* MatEsp (int n, double **A)
{
    /* Declaração de variáveis */
    int i, j;
    SparseMatrix *ini, *cellA;

    /* Criação da célula inicial */
    ini = newCell (0, 0, 0);

    /* Criação da lista ligada referente à matriz esparsa */
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (A[i][j] != 0)
            {
                if (ini->num == 0)
                    ini = newCell (A[i][j], i, j);
                else
                {
                    for (cellA = ini; cellA->prox != NULL; cellA = cellA->prox);
                    cellA->prox = newCell (A[i][j], i, j);
                }
            }
        }
    }

    return ini;
}

/* Multiplicação matriz x vetor             */
/* A matriz é dadad como uma lista ligada   */
/* Retorna um VETOR                         */
double* MatxVec (int n, SparseMatrix *A, double *v)
{
    /* Declaração de variáveis */
    int i;
    double *res, sum; /* res = result */
    SparseMatrix *ini;

    /* Criação do vetor que receberá o resultado da multiplicação */
    res = malloc (n * sizeof (double));
    for (i = 0; i < n; i++)
        res[i] = 0;

    sum = 0;

    /* Multiplicação matriz x vetor */
    for (ini = A; ini != NULL; ini = ini->prox)
    {
        sum = sum + ini->num * v[ini->i];

        /* Verifica se está indo para a "próxima linha" da matriz esparsa */
        if (ini->prox != NULL)
        {
            if (ini->i < ini->prox->i)
            {
                res[ini->i] = sum;
                sum = 0;
            }
        }
        else
            res[ini->i] = sum;
    }

    return res;
}

/* Multiplicação (vetor^T) x vetor */
/* Retorna um NÚMERO               */
double VecxVec (int n, double *v, double *u)
{
    /* Declaração de variáveis */
    int i;
    double res;

    res = 0;

    /* Multiplicação da transposta do vetor v pelo vetor u */
    for (i = 0; i < n; i++)
        res = res + v[i] * u[i];

    return res;
}

/* Multiplicação constante escalar x vetor */
/* Retorna um VETOR                        */
double* cxVec (int n, double *v, double k)
{
    /* Declaração das variáveis */
    int i;
    double *res; /* res = result */

    /* Criação do vetor que receberá o resultado da multiplicação constante x vetor */
    res = malloc (n * sizeof (double));

    /* Multiplicação da constante pelo vetor */
    for (i = 0; i < n; i++)
        res[i] = v[i] * k;

    return res;
}

/* Verifica se o vetor r, referente ao resíduo da subtração b - Ax, tende a zero */
int is_r_0 (int n, double *r)
{
    /* Declaração de variáveis */
    int flag, i;
    double sum;

    sum = 0;
    flag = 0;

    /* Calculo da norma-2 do vetor r */
    for (i = 0; i < n; i++)
        sum = sum + (fabs(r[i]))*(fabs(r[i]));

    sum = sqrt(sum);

    if (sum == 0)
        flag = 1;

    return flag;
}
