/*
Vítor Kei Taira Tamada - 8516250
*/

#include <bits/stdc++.h>
#include <readline/readline.h>
using namespace std;

#define MAX 100

/* Vetor que conterá o comando inserido e os argumentos */
char *ARGS[MAX];

int total, virt, s, p;
fstream file;

/* Identificam qual algoritmo de gerenciamento de espaço livre e de substituição de página será executado */
int esp = 0;
int pag = 0;

/* Protótipos */
void arguments(char*);
void commands(char*);
void executa(char*);

int main()
{
  char *CMD;
  char *AUX;
  
  CMD = (char*) malloc(MAX + 1);
  AUX = (char*) malloc(MAX + 1);
  sprintf(AUX, "(ep3): ");
  
  while(true)
  {
    CMD = readline(AUX);
    
    if(!strcmp(CMD, "sai"))
    {
      if(file.is_open()) file.close();
      break;
    }
    else commands(CMD);
  }
}

/*
 * Função reutilizada do EP1.
 * Armazena o comando em ARGS[0] e o comando em ARGS[1]
 */
void arguments(char *input)
{
  int i, l;
  char *token;
  
  for(i = 0; i < 21 && ARGS[i] != NULL; i++)
  {
    free(ARGS[i]);
    ARGS[i] = NULL;
  }
  
  i = 0;
  token = strtok(input, " ");
  while(token != NULL)
  {
    l = strlen(token);
    ARGS[i] = (char*) malloc(l + 1);
    strcpy(ARGS[i], token);
    token = strtok(NULL, " ");
    ++i;
  }
}

/*
 * Recebe o comando inserido e o executa
 */
void commands(char *input)
{
  arguments(input);
  if     (!strcmp(ARGS[0], "carrega"))    file.open (ARGS[1], ios::in);
  else if(!strcmp(ARGS[0], "espaco"))     esp = stoi(ARGS[1]);
  else if(!strcmp(ARGS[0], "substitui"))  pag = stoi(ARGS[1]);
  else if(!strcmp(ARGS[0], "executa"))    executa   (ARGS[1]);
}

void executa(char *intervalo)
{
  
}
