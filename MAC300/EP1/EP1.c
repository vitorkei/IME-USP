/*
    MAC 300 - Métodos Numéricos de Álgebra Linear

    Nome: Vítor Kei Taira Tamada
    NUSP: 8516250
    Exercício-programa 1 - Resolução de sistemas de equações lineares

    Comando de compilação:
        gcc -Wall -ansi -pedantic -O2 -o EP1 EP1.c -lm

    Notas:
        -Por algum motivo, apenas no meu computador pessoal, que roda Ubuntu 14.04, aparece uma série de warnings do tipo "ignoring return value of..." enquanto nos computadores da Rede Linux, que rodam Debian, não apresentam esse problema. Isso não parece afetar o funcionamento do programa, mas também não sei como resolvê-lo
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/**********************/
/* Funções auxiliares */
/**********************/

/* OK */void print (int, double**, double*);
/* OK */void teste (int, double**, double*, int*);

/*************************************************/
/* Primeira parte - sistemas definidos positivos */
/*************************************************/

/* Implementação orientada a coluna */
/* OK */int cholcol (int, double**);
/* OK */int forwcol (int, double**, double*);
/* OK */int backcol (int, double**, double*, int);

/* Implementação orientada a linha */
/* OK */int cholrow (int, double**);
/* OK */int forwrow (int, double**, double*);
/* OK */int backrow (int, double**, double*, int);

/***********************************/
/* Segunda parte - sistemas gerais */
/***********************************/

/* Implementação orientada a coluna */
/* OK */int lucol (int, double**, int*);
/* OK */int sscol (int, double**, int*, double*);

/* Implementação orientada a linha */
/* OK */int lurow (int, double**, int*);
/* OK */int ssrow (int, double**, int*, double*);

int main ()
{
    /* Declaração de variáveis */
    FILE *file;
    char filePath[100];
    int i, j, n, aux, *p;
    double **A, *b, num;

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
    for (aux = 0; aux < n * n; aux++)
    {
        fscanf (file, "%d", &i);
        fscanf (file, "%d", &j);
        fscanf (file, "%lf", &num);
        A[i][j] = num;
    }

    /* Cria o vetor b[n] */
    b = malloc (n * sizeof (double));
    for (aux = 0; aux < n; aux++)
    {
        fscanf (file, "%d", &i);
        fscanf (file, "%lf", &num);
        b[i] = num;
    }

    /* Fecha o arquivo de entrada */
    fclose (file);

    /* Cria o vetor de permutação p[n] */
    p = malloc (n * sizeof (int));

    /* Testa as funções */
    teste (n, A, b, p);

    /* Libera a memória alocada para as matrizes e vetores */
    for (i = 0; i < n; i++)
        free (A[i]);

    free (A);
    free (b);
    free (p);

    return 0;
}

/*********************************/
/*                               */
/******* Funções auxiliares ******/
/*                               */
/*********************************/

/* Função que imprime a matriz de entrada A e o vetor de saída x. Função separada para ficar mais organizado */
void print (int n, double **A, double *b)
{
    /* Declaração de variáveis */
    int i, j;

    /* Imprime a dimensão da matriz */
    printf ("\nn = %d\n\n", n);

    /* Imprime A */
    printf ("A:\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
            printf ("A[%d][%d] = %f\t", i, j, A[i][j]);
        printf ("\n");
    }

    /* Imprime x */
    printf ("\nx:\n");
    for (i = 0; i < n; i++)
        printf ("x[%d] = %f\n", i, b[i]);
}

/* Função que testa as funções do programa */
void teste (int n, double **A, double *b, int *p)
{
    /* Declaração de variáveis */
    int suc, opt, trans;

    /* suc (sucesso) inicializado como 0 */
    suc = 0;

    printf ("\nEscolha o tipo de resolução:\n\n1) Cholesky orientado à linha\n2) Cholesky orientado à coluna\n3) LU orientado à linha\n4) LU orientado à coluna\n\n");
    scanf ("%d", &opt); /* opt = option */

    /* INFORMAÇÃO VÁLIDA PARA QUALQUER UMA DAS RESOLUÇÕES ESCOLHIDAS */
    /* Se qualquer uma das funções retornar -1 (suc == -1), a resolução é parada */
    if (opt == 1)
    {
        suc = cholrow (n, A);

        if (suc == -1)
            printf ("\n***** A matriz A não é definida positiva *****\n");

        else
        {
            suc = forwrow (n, A, b);

            if (suc == -1)
                printf ("\n***** A matriz A é singular *****\n");

            else
            {
                printf ("\nEscolha o valor de trans (0 ou 1)\n");
                scanf ("%d", &trans);

                if (trans == 0 || trans == 1)
                {
                    suc = backrow (n, A, b, trans);

                    if (suc == -1)
                        printf ("\n***** A matriz A é singular *****\n");
                }

                else
                    printf ("\n***** Valor inválido para trans *****\n");
            }
        }
    }

    else if (opt == 2)
    {
        suc = cholcol (n, A);

        if (suc == -1)
            printf ("\n***** A matriz A não é definida positiva *****\n");

        else
        {
            suc = forwcol (n, A, b);

            if (suc == -1)
                printf ("\n***** A matriz A é singular *****\n");

            else
            {
                printf ("\nEscolha o valor de trans (0 ou 1)\n");
                scanf ("%d", &trans);

                if (trans == 0 || trans == 1)
                {
                    suc = backcol (n, A, b, trans);

                    if (suc == -1)
                        printf ("\n***** A matriz A é singular *****\n");
                }

                else
                    printf ("\n***** Valor inválido para trans *****\n");
            }
        }
    }

    else if (opt == 3)
    {
        suc = lurow (n, A, p);

        if (suc == -1)
            printf ("\n***** A matriz A é singular *****\n");

        else
        {
            suc = ssrow (n, A, p, b);

            if (suc == -1)
                printf ("\n***** A matriz U é singular *****\n");
        }
    }

    else if (opt == 4)
    {
        suc = lucol (n, A, p);

        if (suc == -1)
            printf ("\n***** A matriz A é singular *****\n");

        else
        {
            suc = sscol (n, A, p, b);

            if (suc == -1)
                printf ("\n***** A matriz U é singular *****\n");
        }
    }

    else
        printf ("\n***** Opção inválida *****\n");

    if (opt > 0 && opt < 5 && suc == 0)
        print (n, A, b);

    printf ("\nPara imprimir a matriz de entrada A e o vetor de saída x, basta \"descomentar\" o if da linha 267 do código\n\n");
}

/***********************************************************/
/*                                                         */
/****** Primeira parte - sistemas definidos positivos ******/
/*                                                         */
/***********************************************************/

/**********************************************/
/* *****Implementação orientada à coluna***** */
/**********************************************/

/* Decomposição de Cholesky orientado à coluna */
int cholcol (int n, double **A)
{
    /* Declaração de variáveis */
    int i, j, k, suc;

    /* Variáveis para medir o tempo levado para executar esta função */
    clock_t begin, end;
    double time_spent;

    begin = clock ();

    /* suc (sucesso) em realizar a decomposição de Cholesky */
    suc = 0;

    /* Decomposição de Cholesky pelo produto externo */
    for (k = 0; k < n && suc == 0; k++)
    {
        if (A[k][k] <= 0)
            suc = -1;
        else
        {
            A[k][k] = sqrt (A[k][k]);

            for (i = k + 1; i < n; i++)
                A[i][k] /= A[k][k];

            for (i = k + 1; i < n; i++)
                for (j = k + 1; j <= i; j++)
                    A[i][j] -= A[i][k] * A[j][k];
        }
    }

    end = clock ();

    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf ("\ncholcol: %f segundos\n", time_spent);

    return suc;
}

/* Implementação orientada à coluna pela frente */
int forwcol (int n, double **A, double *b)
{
    /* Declaração de variáveis */
    int i, j, suc;

    /* Variáveis para medir o tempo levado para executar esta função */
    clock_t begin, end;
    double time_spent;

    begin = clock ();

    /* suc (sucesso) em resolver o sistema */
    suc = 0;

    for (j = 0; j < n && suc == 0; j++)
    {
        if (A[j][j] == 0)
            suc = -1;
        else
        {
            b[j] /= A[j][j];

            for (i = j + 1; i < n; i++)
                b[i] -= A[i][j] * b[j];
        }
    }

    end = clock ();

    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf ("forwcol: %f segundos\n", time_spent);

    return suc;
}

/* Implementação orientada à coluna por trás */
int backcol (int n, double **A, double *b, int trans)
{
    /* Declaração de variáveis */
    int i, j, suc;

    /* Variáveis para medir o tempo levado para executar esta função */
    clock_t begin, end;
    double time_spent;

    begin = clock ();

    /* suc (sucesso) em resolver o sistema */
    suc = 0;

    /* Resolve o sistema considerando A como uma matriz triangular superior */
    if (trans == 0)
        for (j = n - 1; j > -1 && suc == 0; j--)
        {
            if (A[j][j] == 0)
                suc = -1;
            else
            {
                b[j] /= A[j][j];

                for (i = j - 1; i > -1; i--)
                    b[i] -= A[i][j] * b[j];
            }
        }

    /* A verificação de se trans é 0 ou 1 é feita fora desta função. Logo, se trans não for 0, com certeza será 1 */
    else
        for (i = n - 1; i > -1 && suc == 0; i--)
        {
            for (j = n - 1; j > i; j--)
                b[i] -= A[j][i] * b[j];

            if (A[i][i] == 0)
                suc = -1;
            else
                b[i] /= A[i][i];
        }

    end = clock ();

    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf ("\nbackcol = %f segundos\n", time_spent);

    return suc;
}

/***********************************************/
/* *****Implementações orientadas à linha***** */
/***********************************************/

/* Decomposição de Cholesky orientada à linha */
int cholrow (int n, double **A)
{
    /* Declaração de variáveis */
    int i, j, k, suc;

    /* Variáveis para medir o tempo levado para executar esta função */
    clock_t begin, end;
    double time_spent;

    begin = clock ();

    /* suc (sucesso) em realizar a decomposição de Cholesky */
    suc = 0;

    /* Decomposição de Cholesky pelo produto interno */
    for (j = 0; j < n && suc == 0; j++)
    {
        for (k = 0; k < j; k++)
            A[j][j] -= (A[j][k] * A[j][k]);

        if (A[j][j] <= 0)
            suc = -1;
        else
        {
            A[j][j] = sqrt (A[j][j]);

            for (i = j + 1; i < n; i++)
            {
                for (k = 0; k < j; k++)
                    A[i][j] -= A[i][k] * A[j][k];

                A[i][j] /= A[j][j];
            }
        }
    }

    end = clock ();

    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf ("\ncholrow: %f segundos\n", time_spent);

    return suc;
}

/* Implementação orientada à linha pela frente */
int forwrow (int n, double **A, double *b)
{
    /* Declaração de variáveis */
    int i, j, suc;

    /* Variáveis para medir o tempo levado para executar esta função */
    clock_t begin, end;
    double time_spent;

    begin = clock ();

    /* suc (sucesso) em resolver o sistema */
    suc = 0;

    /* Cálculo do sistema */
    for (i = 0; i < n && suc == 0; i++)
    {
        for (j = 0; j < i; j++)
            b[i] -= A[i][j] * b[j];

        if (A[i][i] == 0)
            suc = -1;
        else
            b[i] /= A[i][i];
    }

    end = clock ();

    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf ("forwrow: %f segundos\n", time_spent);

    return suc;
}

/* Implementação orientada à linha por trás */
int backrow (int n, double **A, double *b, int trans)
{
    /* Declaração de variáveis */
    int i, j, suc;

    /* Variáveis para medir o tempo levado para executar esta função */
    clock_t begin, end;
    double time_spent;

    begin = clock ();

    /* suc (sucesso) em resolver o sistema */
    suc = 0;

    /* Resolve o sistema considerando A como uma matriz triangular superior */
    if (trans == 0)
        for (i = n - 1; i > -1 && suc == 0; i--)
        {
            for (j = n - 1; j > i; j--)
                b[i] -= A[i][j] * b[j];

            if (A[i][i] == 0)
                suc = -1;
            else
                b[i] /= A[i][i];
        }

    /* A verificação de se trans é 0 ou 1 é feita fora desta função. Logo, se trans não for 0, com certeza será 1 */
    else
        for (j = n - 1; j > -1 && suc == 0; j--)
        {
            if (A[j][j] == 0)
                suc = -1;
            else
            {
                b[j] /= A[j][j];

                for (i = j - 1; i > -1; i--)
                    b[i] -= A[j][i] * b[j];
            }
        }

    end = clock ();

    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf ("\nbackrow: %f segundos\n", time_spent);

    return suc;
}

/*********************************************/
/*                                           */
/****** Segunda parte - sistemas gerais ******/
/*                                           */
/*********************************************/

/************************************************/
/* *****Implementações orientadas à coluna***** */
/************************************************/

/* Decomposição LU orientada à coluna */
int lucol (int n, double **A, int *p)
{

    /* Método como visto em aula */
    int i, j, k, i_max, suc;
    double tmp; /* tmp = temporário */

    /* Variáveis para medir o tempo levado para executar esta função */
    clock_t begin, end;
    double time_spent;

    begin = clock ();

    /* suc (sucesso) em realizar a decomposição LU */
    suc = 0;

    /* Decomposição LU orientada à coluna */
    for (k = 0; k < n - 1 && suc == 0; k++)
    {
        i_max = k;

        for (i = k + 1; i < n; i++)
            if (fabs (A[i][k]) > fabs (A[i_max][k]))
                i_max = i;

        p[k] = i_max;

        if (p[k] != k)
            for (j = 0; j < n; j++)
            {
                tmp = A[k][j];
                A[k][j] = A[p[k]][j];
                A[p[k]][j] = tmp;
            }

        if (A[k][k] == 0)
            suc = -1;
        else
        {
            for (i = k + 1; i < n; i++)
                A[i][k] /= A[k][k];

            /* flop que determina que a decomposição é feita em colunas */
            for (j = k + 1; j < n; j++)
                for (i = k + 1; i < n; i++)
                    A[i][j] -= A[i][k] * A[k][j];
        }
    }

    /* Verificação para o elemento a[n][n]. Encontra-se separada por não precisar realizar permutação */
    if (A[n - 1][n - 1] == 0)
        suc = -1;
    else
        p[n - 1] = n;

    end = clock ();

    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf ("\nlucol: %f segundos\n", time_spent);

    return suc;
}

/* Cálculo de x em Ax=b, pela decomposição LU, orientada à coluna */
int sscol (int n, double **A, int *p, double *b)
{
    /* Declaração de variáveis */
    int i, j, suc;
    double tmp; /* tmp = temporário */

    /* Variáveis para medir o tempo levado para executar esta função */
    clock_t begin, end;
    double time_spent;

    begin = clock ();

    /* suc (sucesso) em realizar o sistema */
    suc = 0;

    /* Cálculo de Pb, sendo P o vetor *p (vetor de permutação) e b o vetor *b */
    for (i = 0; i < n - 1; i++)
    {
        tmp = b[i];
        b[i] = b[p[i]];
        b[p[i]] = tmp;
    }

    /* Cálculo de y na equação Ly = Pb (pela frente e orientado à coluna) */
    for (j = 0; j < n - 1; j++)
        for (i = j + 1; i < n; i++)
            b[i] -= A[i][j] * b[j];

    /* Cálculo de x na equação Ux = y (por trás e orientado à coluna) */
    for (j = n - 1; j > -1 && suc == 0; j--)
    {
        if (A[j][j] == 0)
            suc = -1;
        else
        {
            b[j] /= A[j][j];

            for (i = 0; i < j; i++)
                b[i] -= A[i][j] * b[j];
        }
    }

    end = clock ();

    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf ("sscol: %f segundos\n", time_spent);

    return suc;
}

/***********************************************/
/* *****Implementações orientadas à linha***** */
/***********************************************/

/* Decomposição LU orientada à linha */
/* p[k] recebe o número da linha que contém o maior número em módulo da coluna k */
int lurow (int n, double **A, int *p)
{
    /* Declaração de variáveis */
    int i, j, k, a_max, suc;
    double tmp; /* tmp = temporário */

    /* Variáveis para medir o tempo levado para executar esta função */
    clock_t begin, end;
    double time_spent;

    begin = clock ();

    /* suc (sucesso) em realizar a decomposição LU */
    suc = 0;

    /* Decomposição LU orientada à linha */
    for (k = 0; k < n - 1 && suc == 0; k++)
    {
        a_max = k;

        for (i = k + 1; i < n; i++)
            if (fabs (A[i][k]) > fabs (A[a_max][k]))
                a_max = i;

        p[k] = a_max;

        if (p[k] != k)
            for (j = 0; j < n; j++)
            {
                tmp = A[k][j];
                A[k][j] = A[p[k]][j];
                A[p[k]][j] = tmp;
            }

        if (A[k][k] == 0)
            suc = -1;
        else
            /* flop que determina que a decomposição é feita em linhas */
            for (i = k + 1; i < n; i++)
            {
                A[i][k] /= A[k][k];
                for (j = k + 1; j < n; j++)
                    A[i][j] -= A[i][k] * A[k][j];
            }
    }

    /* Verificação para o elemento a[n][n]. Encontra-se separada por não precisar realizar permutação */
    if (A[n - 1][n - 1] == 0)
        suc = -1;
    else
        p[n - 1] = n;

    end = clock ();

    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf ("\nlurow: %f segundos\n", time_spent);

    return suc;
}

/* Cálculo de x em Ax=b, pela decomposição LU, orientada à linha */
int ssrow (int n, double **A, int *p, double *b)
{
    /* Declaração de variáveis */
    int i, j, suc;
    double tmp; /* tmp = temporário */

    /* Variáveis para medir o tempo levado para executar esta função */
    clock_t begin, end;
    double time_spent;

    begin = clock ();

    /* suc (sucesso) em resolver o sistema */
    suc = 0;

    /* Cálculo de Pb, sendo P o vetor *p (vetor de permutação) e b o vetor *b */
    for (i = 0; i < n - 1; i++)
    {
        tmp = b[i];
        b[i] = b[p[i]];
        b[p[i]] = tmp;
    }

    /* Cálculo de y na equação Ly = Pb (pela frente e orientado à linha) */
    for (i = 0; i < n && suc == 0; i++)
    {
        for (j = 0; j < i; j++)
            b[i] -= A[i][j] * b[j];

        if (A[i][i] == 0)
            suc = -1;
    }

    /* Cálculo de x na equação Ux = y (por trás e orientado à linha) */
    for (i = n - 1; i > -1 && suc == 0; i--)
    {
        for (j = n - 1; j > i; j--)
            b[i] -= A[i][j] * b[j];

        if (A[i][i] == 0)
            suc = -1;
        else
            b[i] /= A[i][i];
    }

    end = clock ();

    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf ("ssrow: %f segundos\n", time_spent);

    return suc;
}
