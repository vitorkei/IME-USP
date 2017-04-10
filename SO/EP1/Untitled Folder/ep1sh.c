/*************************************************************/
/*                                                           */
/* MAC0422 - Sistemas Operacionais - 2016/2                  */
/*                                                           */
/* Nome: Vítor Kei Taira Tamada                              */
/* NUSP: 8516250                                             */
/* Exercício-programa 1 - shell                              */
/*                                                           */
/*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <math.h>

/* Tamanho máximo de caracteres do caminho, comando recebido, ... */
#define MAX 500
#define TRUE 1

/*

ARGS[0] contém o comando
ARGS[i] contémo o i-ésimo argumento do comando

*/
char *ARGS[21];

/* Separa o comando e os argumentos em espaços no vetor ARGS */
void arguments(char *cmd)
{
  int i, l;
  char *token;

  /* Libera o vetor ARGS e faz todos os seus espaços apontarem para NULL */
  for (i = 0; i < 21 && ARGS[i] != NULL; i++)
  {
    free(ARGS[i]);
    ARGS[i] = NULL;
  }

  /* Aloca o comando em ARGS[0] e cada argumento em uma casa do vetor */
  i = 0;
  token = strtok(cmd, " ");
  while (token != NULL)
  {
    l = strlen(token);
    ARGS[i] = malloc((l + 1) * sizeof(char));
    strcpy(ARGS[i], token);
    token = strtok(NULL, " ");
    i++;
  }
}

/*

Converte um número base-8 em um número base-10
Função utilizada com o comando chmod

*/
int oct_to_dec(int n)
{
  int dec = 0, i = 0, r;
  while (n != 0)
  {
    r = n % 10;
    n /= 10;
    dec += r * pow(8, i);
    i++;
  }
  return dec;
}

int main()
{
  char *PATH;           /* Caminho para o diretório em que o shell está sendo executado */
  char *CMD;            /* Comando inserido pelo usuário no shell                       */
  char *AUX;            /* Variável utilizada para imprimir "(PATH): " mais facilmente  */
  char *ENV[] = {NULL}; /* Terceiro argumento do execve                                 */
  pid_t pid;            /* process ID                                                   */

  /* Aloca o espaço utilizado para cada variável */
  PATH = malloc(MAX * sizeof(char));
  CMD = malloc(MAX * sizeof(char));
  AUX = malloc((MAX + 4) * sizeof(char));

  /* Inicializa a função de histórico do shell */
  using_history();
  while (TRUE)
  {
    /* Recebe PATH e inicializa AUX como "(PATH): " */
    getcwd(PATH, MAX);
    sprintf(AUX, "(%s/): ", PATH);

    /* Recebe comando e adiciona ao histórico */
    CMD = readline(AUX);
    add_history(CMD);

    /* Separa comando de argumentos */
    arguments(CMD);

    /* Se estiver no processo filho */
    if ((pid = fork()) == 0)
    {
      /* Se o comando inserido for o chmod */
      if (strcmp(ARGS[0], "chmod") == 0)
      {
        int num = ((((int) ARGS[1][0]) - 48) * 100) + ((((int) ARGS[1][1]) - 48) * 10) + (((int) ARGS[1][2]) - 48);
        num = oct_to_dec(num);
        chmod(ARGS[2], num);
      }

      /* Se o comando inserido for o id */
      else if (strcmp(ARGS[0], "id") == 0)
      {
        uid_t id;
        id = getuid();
        printf("%d\n", id);
      }

      /* Se o comando inserido não for nenhum dos acima */
      else
        execve(ARGS[0], ARGS, ENV);
    }

    /* Se der erro e acessar um process ID inválido, retorna uma mensagem de erro e encerra o programa */
    else if (pid == -1)
    {
      printf("Error occurred: process ID = -1\n");
      return -1;
    }

    /* Se estiver no processo pai, apenas aguarda que o processo filho termine */
    else
      waitpid(pid, NULL, 0);
  }

  return 0;
}
