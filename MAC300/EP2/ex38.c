/*
    Programa que gera o exemplo do capítulo 38 do livro
    Lloyd N. Trefethen, David Bau, Numerical Linear Algebra, Society for Industrial and Applied
Mathematics, Philadelphia, 1997

    Usei um pouco o genmatsim.c do EP1 para fazer este programa

    Código de compilação:
        gcc -Wall -o ex38 ex38.c -lm
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define nmax 1000

int main ()
{
    FILE *file;
    int i, j, n;
    double A[nmax][nmax], b[nmax], param, div, rand_num; /* param = parameter */
    char fileName[100];

    printf ("Defina um n que será a dimensão da matriz a ser criada: ");
    scanf ("%d", &n);

    /* Para gerar um número aleatório dentro do intervalo [-1, 1] */
    srand (time (NULL));
    div = RAND_MAX / 2;

    printf ("Defina um parâmetro: ");
    scanf ("%lf", &param);

    printf ("Defina um nome para o arquivo de saída: ");
    scanf ("%s", fileName);

    /* Constrói a matriz A segundo o exemplo do capítulo 38 do livro */
    for (i = 0; i < n; i++)
    {
        for (j = i; j < n; j++)
        {
            if (i == j)
                A[i][j] = 1;
            else
            {
                rand_num = -1 + (rand() / div);
                if (fabs(rand_num) > param)
                {
                        A[i][j] = 0;
                        A[j][i] = 0;
                }
                else
                {
                    A[i][j] = rand_num;
                    A[j][i] = rand_num;
                }
            }
        }
    }

    /* Gera o vetor b */
    for (i = 0; i < n; i++)
    {
        rand_num = -1 + (rand() / div);
        b[i] = rand_num;
    }

    file = fopen (fileName, "wt");
    fprintf (file, "%d", n);

    for (i = 0; i < n; i++)
        if (i == n - 1)
            for (j = 0; j < n; j++)
                fprintf (file, "\n%3d %3d %f", i, j, A[0][j]);
        else
            for (j = 0; j < n; j++)
                fprintf (file, "\n%3d %3d %f", i, j, A[i][j]);

    for (i = 0; i < n; i++)
        fprintf (file, "\n%3d %f", i, b[i]);

    fclose (file);

    return 0;
}
