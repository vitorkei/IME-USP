/*
* MAC0422 – Sistemas Operacionais - 2016/2
* 
* Nome: Vítor Kei Taira Tamada
* NUSP: 8516250
* 
* Professor: Daniel
* Exercício Programa 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX 500
#define TRUE 1

char *ARGS[21];

void arguments(char *cmd)
{
  int i, l;
  char *token;

  for (i = 0; i < 21 && ARGS[i] != NULL; i++)
  {
    free(ARGS[i]);
    ARGS[i] = NULL;
  }

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

int main()
{
  char *PATH;
  char *CMD;
  char *AUX;
  char *ENV[] = {NULL};
  pid_t pid;

  PATH = malloc(MAX * sizeof(char));
  CMD = malloc(MAX * sizeof(char));
  AUX = malloc((MAX + 4) * sizeof(char));

  using_history();

  while (TRUE)
  {
    getcwd(PATH, MAX);
    sprintf(AUX, "(%s): ", PATH);

    CMD = readline(AUX);
    add_history(CMD);

    arguments(CMD);

    if ((pid = fork()) == 0)
    {
      if (strcmp(ARGS[0], "chmod") == 0)
      {
        
      }
      else if (strcmp(ARGS[0], "id") == 0)
      {

      }
      else
      {
        execve(ARGS[0], ARGS, ENV);
      }
    }
    else if (pid == -1)
    {
      printf("Error occurred: process ID = -1\n");
      return -1;
    }
    else
    {
      waitpid(pid, NULL, 0);
    }
  }

  return 0;
}
