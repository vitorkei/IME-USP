/*************************************************************/
/*                                                           */
/* MAC0422 - Sistemas Operacionais - 2016/2                  */
/*                                                           */
/* Nome: Vítor Kei Taira Tamada                              */
/* NUSP: 8516250                                             */
/* Exercício-programa 1 - Simulador de processos             */
/*                                                           */
/*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/*************************************************************/
/*                                                           */
/*                          Structs                          */
/*                                                           */
/*************************************************************/

/*
* Struct que contém as propriedades
*   do processo segundo especificado
*   pelo enunciado
*/
typedef struct p
{
  double t0;
  char name[20];
  double dt;
  double dl; /* deadline */

  double tr;
  double tf;

  int status;
  /*
   *  -1 = Não chegou
   *   0 = Standby
   *   1 = Processando
   *   2 = Terminado
   */

  pthread_t tid;  /* thread ID */
  int tl;         /* trace line - linha do processo no arquivo de trace */
  double pDelta;  /* Tempo que o processo rodou até o momento*/
} process;

/*
* Struct que servirá para fazer uma lista ligada
*   de processos
*/
typedef struct cel
{
  process *proc;
  struct cel *next;
} cell;

/****************************************************************/
/*                                                              */
/*                          Protótipos                          */
/*                                                              */
/****************************************************************/

/* Auxiliares */
/* OK */ cell *newCell(double, char*, double, double, int);
/* OK */ void timeOP();
/* OK */ double deltaTime(clock_t, clock_t);
/* OK */ void printOutput(cell*, FILE*);
/* OK */ void removeFirstCell(cell*);
/* OK */ void freeList(cell*);

/* FCFS */
/* OK */ void FCFS();
/* OK */ void *exec_proc_FCFS(void*);
/* OK */ void insertFCFS(cell*, cell*);
/* OK */ void parseFCFS(char*, cell*);

/* SRTN */
void SRTN();
void *exec_proc_SRTN(void*);
/* OK */ void insertSRTN(cell*, cell*);
/* OK */ void parseSRTN(char*, cell*);
/* OK */ void SRTNInsert(cell*, cell*);

/* EMF */
void EMF();

/**********************************************************************/
/*                                                                    */
/*                          Variáveis globais                         */
/*                                                                    */
/**********************************************************************/

cell *proc_list;      /* Apontador para a cabeça da lista ligada */
cell *proc_list_SRTN; /* Lista ligada utilizada apenas pelo SRTN */

clock_t start;        /* start - quando o programa começou a rodar */
double delta = -1;
int DEBUG = 0, proc_count = 0, MAX_PROCS, MAX_THREADS, thread_count, count = 0;
int ctxt_swt_count = 0; /* Contador de mudança de contexto */
sem_t mutex;

int deadline_error_count = 0;
double deadline_error_prop;

/**********************************************************************/
/*                  |\  /|   /\   | |\  |                             */
/*                  | \/ |  /--\  | | \ |                             */
/*                  |    | /    \ | |  \|                             */
/**********************************************************************/

int main(int argc, char **argv)
{
  /* Declaração de variáveis */
  int i, seqCon;  /* seqCon = sequential control = escalonador        */
  cell *head;     /* Aponta para o primeiro item não-cabeça da lista  */
  FILE *out;
  
  /* Inicialização do tempo do programa */
  start = clock();

  /* Inicialização da lista ligada que conterá os processos */
  proc_list = malloc(sizeof(cell));
  proc_list->next = NULL;

  /* Recebe o número máximo de processadores disponíveis */
  MAX_THREADS = sysconf(_SC_NPROCESSORS_ONLN);

  /* Inicializa o semáforo mutex */
  sem_unlink("mutex");
  sem_init(&mutex, 0, 1);

  /* Primeiro argumento determina qual o tipo de escalonador será usado */
  /* Segundo argumento é o arquivo de trace com os processos            */
  /* Terceiro argumento é o arquivo de saída onde serão escritos        */
  seqCon = atoi(argv[1]);
  out = fopen(argv[3],"w");

  /* Verifica se existe o quarto comando d */
  if(argc > 4)
    if(argv[4][0] == 'd')
      DEBUG = 1;

  /* First-Come, First-Served */
  if(seqCon == 1)
  {
    /* parseFCFS é um parse que funciona apenas para o FCFS */
    parseFCFS(argv[2], proc_list);
    FCFS();
  }
  /* Shortest Remaining Time Next */
  else if(seqCon == 2)
  {
    proc_list_SRTN = malloc(sizeof(cell));
    proc_list_SRTN->next = NULL;
    parseSRTN(argv[2], proc_list);
    SRTN();
  }
  /* Escalonamento com múltiplas filas */
  else if(seqCon == 3)
  {
    EMF();
    printf("ERRO: Escalonador não implementado!\n");
    return 1;
  }

  /* Verifica se há processos não terminados */
  i = 0;
  head = proc_list->next;
  while(i < MAX_PROCS)
  {
    pthread_join(head->proc->tid, NULL);
    head = head->next;
    i++;
  }

  /*printOutput(proc_list, out);*/

  if(DEBUG == 1)
    fprintf(stderr, "Número de trocas de contexto: %d\n", ctxt_swt_count);

  deadline_error_prop = ((double) deadline_error_count)/ ((double) MAX_PROCS);
  fprintf(out, "%d %f\n", deadline_error_count, deadline_error_prop);
  /* Fecha o arquivo de saída e libera as listas */
  fclose(out);
  freeList(proc_list);
  if(seqCon == 2) freeList(proc_list_SRTN);
  

  return 0;
}

/************************************************************************/
/*                                                                      */
/*                          Funções auxiliares                          */
/*                                                                      */
/************************************************************************/

/* Cria uma célula do tipo processo */
cell *newCell(double t0, char *name, double dt, double dl, int tl)
{
  cell *c;
  process *p;

  c = malloc(sizeof(cell));
  p = malloc(sizeof(process));

  p->t0 = t0;
  strcpy(p->name, name);
  p->dt = dt;
  p->dl = dl;
  p->tl = tl;

  /* 
   *  Inicializando tr e tf já que são valores que serão
   *    atribuídos mais tarde dependendo do tempo
   */
  p->tr = 0;
  p->tf = 0;
  p->pDelta = 0;
  
  p->status = 0;

  c->proc = p;
  c->next = NULL;

  return c;
}

/* Operação para consumir tempo real da CPU */
void timeOP()
{
  int i, j = 0;
  clock_t lap = clock();
  for(i = 0; i < 50; i++) j += i;

  delta = deltaTime(start, lap);
}

/* Retorna o tempo que passou entre ini e end em segundos */
double deltaTime(clock_t ini, clock_t end)
{
  double deltaT = ((double) (end - ini)) / ((double) CLOCKS_PER_SEC);
  return deltaT;
}

/* Imprime para um arquivo de saída */
void printOutput(cell *head, FILE *out)
{
  cell *p;
  p = head->next;
  while(p != NULL)
  {
    fprintf(out, "%s %f %f\n", p->proc->name, p->proc->tf, p->proc->tr);
    p = p->next;
  }
  fprintf(out, "%d\n", ctxt_swt_count);
}

/* Remove o primeiro elemento não-cabeça da lista */
void removeFirstCell(cell *head)
{
  cell *p, *q, *f;
  
  p = head;
  f = head->next;
  q = f->next;
  p->next = q;
  
  free(f);
}

/* Libera a lista */
void freeList(cell *head)
{
  cell *p, *q;
  
  p = head->next;
  while (p != NULL)
  {
    q = p->next;
    free(p->proc);
    free(p);
    p = q;
  }
  free(head->proc);
  free(head);
}

/***********************************************************************/
/*                                                                     */
/*                   First-Come, First-Served (FCFS)                   */
/*                                                                     */
/***********************************************************************/

/* Escalonamento First Come, First Served (FCFS) */
void FCFS()
{
  cell *c;
  c = proc_list->next;

  /* Percorre todos os processos */
  while(c != NULL)
  {
    /*
    *  Executa uma ação que consome tempo real da CPU
    *    até que o próximo processo chegue
    */
    while(c->proc->t0 > delta) timeOP();

    if(DEBUG == 1)
      fprintf(stderr, "\n+ Processo de nome %s e linha %d no arquivo de trace acabou de chegar.\n", c->proc->name, c->proc->tl);

    /*
    *  Consome tempo real da CPU até que uma das threads
    *    seja liberada caso todas estejam ocupadas
    */
    while(thread_count >= MAX_THREADS) timeOP();

    /* Começo da sessão crítica */
    sem_wait(&mutex);

    /* Cria mais uma thread para ser utilizada */
    thread_count++;
    pthread_create(&c->proc->tid, NULL, exec_proc_FCFS, (void*) c->proc);

    /* Final da sessão crítica */
    sem_post(&mutex);

    c = c->next;
  }
}

/* Executa o processo do escalonador FCFS */
void *exec_proc_FCFS(void *args)
{
  process *p;
  clock_t aux;

  /* Início da sessão crítica */
  sem_wait(&mutex);

  p = (process*) args;
  if(DEBUG == 1)
    fprintf(stderr, "\nProcesso de nome %s está utilizando a CPU 1.\n", p->name);
    
  /* Final da sessão crítica */
  sem_post(&mutex);

  /* Mantém-se no processo até que ele termine de ser executado */
  while(p->pDelta < p->dt)
  {
    aux = clock();
    p->pDelta += deltaTime(aux, clock());
  }

  /* Início de outra sessão crítica */
  sem_wait(&mutex);

  /* 
   * É interessante que o momento que o processo terminou
   *   de rodar seja armazenado com precisão. Logo, esta operação
   *   está dentro da sessão crítica
   */
  p->tf = deltaTime(start, clock());
  p->tr = p->tf - p->t0;

  if(p->tr > p->dl)
    deadline_error_count++;

  thread_count--;
  count++;
  
  if(DEBUG == 1)
    fprintf(stderr, "\n- Processo de nome %s terminou de rodar e será escrito na linha %d do arquivo de saída.\n", p->name, count);

  /* Final da sessão crítica */
  sem_post(&mutex);

  proc_count++;

  return NULL;
}

/* Insere células na lista ligada em ordem crescente de t0 */
void insertFCFS(cell *new, cell *head)
{
  cell *p, *q;

  p = head;
  if(p == NULL)
    head = new;
  else
  {
    q = head->next;
    while(q != NULL && new->proc->t0 > q->proc->t0)
    {
      p = q;
      q = q->next;
    }
    new->next = q;
    p->next = new;
  }
}

/* Recebe os processos do arquivo de trace e os coloca em uma fila por ordem de chegada (t0) */
void parseFCFS(char *trace, cell *proc_list)
{
  cell *c;
  FILE *file;
  int i;
  double t0, dt, dl;
  char aux[100], name[50];

  /* Abre o arquivo de trace */  
  file = fopen(trace, "r");
  
  /* Conta o número de processos do arquivo de trace pelo número de linhas */
  for(i = 0; fgets(aux, 100, file) != NULL; i++);
  MAX_PROCS = i;
  
  /* Retornar ao início do arquivo de trace */
  rewind(file);
  
  /* Cria células para cada um dos processos em ordem de chegada (t0) */
  for(i = 0; i < MAX_PROCS; i++)
  {
    fscanf(file, "%lf %s %lf %lf", &t0, name, &dt, &dl);
    c = newCell(t0, name, dt, dl, i);
    insertFCFS(c, proc_list);
  }
  
  fclose(file);
}

/***********************************************************************/
/*                                                                     */
/*                 Shortest Remaining Time Next (SRTN)                 */
/*                                                                     */
/***********************************************************************/

void SRTN()
{
  cell *c;
  c = proc_list->next;

  /* Percorre todos os processos */
  while(c != NULL)
  {
    /*
     *  Executa uma ação que consome tempo real da CPU
     *    até que o próximo processo chegue
     */
    while(c->proc->t0 > delta) timeOP();
    
    if(DEBUG == 1)
      fprintf(stderr, "\n+ Processo de nome %s e linha %d no arquivo de trace acabou de chegar.\n", c->proc->name, c->proc->tl);
    
    /*
     *  Consome tempo real da CPU até que uma das threads
     *    seja liberada caso todas estejam ocupadas
     */

    while(thread_count >= MAX_THREADS) timeOP();

    /* Insere o novo processo na fila de acordo com o tempo restante para finalizar */
    SRTNInsert(c, proc_list_SRTN);
    
    /* Início da sessão crítica */
    sem_wait(&mutex);
    
    thread_count++;
    pthread_create(&c->proc->tid, NULL, exec_proc_SRTN, (void*) c->proc);
    
    /* Final da sessão crítica */
    sem_post(&mutex);
    c = c->next;
  }
}

void *exec_proc_SRTN(void *args)
{
  int flag = 0;
  process *p;
  clock_t aux;
  
  /* Início da sessão crítica */
  sem_wait(&mutex);
  
  p = (process*) args;
  if(DEBUG == 1)
    fprintf(stderr, "\nProcesso de nome %s está utilizando a CPU.\n", p->name);
    
  /* Final da sessão crítica */
  sem_post(&mutex);
  
  /* Verifica se troca de contexto é necessária */
  while(p->pDelta < p->dt)
  {
    /* Início de outra sessão crítica */
    sem_wait(&mutex);
    /*
     *  Verifica se o process que está rodando é o de menor tempo restante.
     *  Utiliza a linha do arquivo de trace onde o processo se encontra
     *    para fazer a comparação.
     */
    if(p->tl != proc_list_SRTN->next->proc->tl)
    {
      flag = 1;
      thread_count--;
      ctxt_swt_count++;
      if(DEBUG == 1)
        fprintf(stderr, "\n<> Processo de nome %s parou e será trocado pelo processo %s\nTrocas de contexto:%d\n", p->name, proc_list_SRTN->next->proc->name, ctxt_swt_count);
    }
    sem_post(&mutex);
    
    if(flag == 1) return NULL;
    
    aux = clock();
    p->pDelta += deltaTime(aux, clock());
  }

  sem_wait(&mutex);
  
  p->tf = deltaTime(start, clock());
  p->tr = p->tf - p->t0;
  p->status = 1;
  
  thread_count--;
  count++;
  
  if(DEBUG == 1)
    fprintf(stderr, "\n- Processo de nome %s terminou de rodar e será escrito na linha %d do arquivo de saída.\n", p->name, count);

  removeFirstCell(proc_list_SRTN);

  sem_post(&mutex);

  proc_count++;

  return NULL;
}

/*
 *  Insere os processos na fila ordenando primeiro
 *    pelo t0 e depois pelo dt
 */
void insertSRTN(cell *new, cell *head)
{
  cell *p, *q;
  
  p = head;
  if(p == NULL)
    head = new;
  else
  {
    q = head->next;
    while(q != NULL && new->proc->t0 > q->proc->t0)
    {
      p = q;
      q = q->next;
    }
    while(q != NULL && new->proc->dt > q->proc->dt)
    {
      p = q;
      q = q->next;
    }
    new->next = q;
    p->next = new;
  }
}

/*
 * Recebe os processos do arquivo de trace e os coloca
 *   em uma fila por ordem de tempo faltando (primeiro
 *   t0 e depois dt)
*/
void parseSRTN(char *trace, cell *proc_list)
{
  cell *c;
  FILE *file;
  int i;
  double t0, dt, dl;
  char aux[100], name[50];
  
  /* Abre o arquivo de trace */  
  file = fopen(trace, "r");
  
  /* Conta o número de processos do arquivo de trace pelo número de linhas */
  for(i = 0; fgets(aux, 100, file) != NULL; i++);
  MAX_PROCS = i;
  
  /* Retornar ao início do arquivo de trace */
  rewind(file);
  
  /* Cria células para cada um dos processos em ordem de chegada (t0) */
  for(i = 0; i < MAX_PROCS; i++)
  {
    fscanf(file, "%lf %s %lf %lf", &t0, name, &dt, &dl);
    c = newCell(t0, name, dt, dl, i);
    insertSRTN(c, proc_list);
  }
  printf("\n");
  
  fclose(file);
}

/*
 * Insere em uma lista baseado no tempo de processamento restante.
 * O primeiro elemento depois da cabeça da lista é o de menor tempo restante.
 */
void SRTNInsert(cell *new, cell *head)
{
  cell *p, *q, *r;
  
  p = head;
  if(p == NULL)
    head = new;
  else
  {
    q = head->next;
    r = newCell(new->proc->t0, new->proc->name, new->proc->dt, new->proc->dl, new->proc->tl);
    while(q != NULL && new->proc->dt > (q->proc->dt - q->proc->pDelta))
    {
      p = q;
      q = q->next;
    }
    if (q != NULL)
      r->next = q;
    p->next = r;
  }
}

/***********************************************************************/
/*                                                                     */
/*                  Escalonamento com múltiplas filas                  */
/*                                                                     */
/***********************************************************************/

void EMF()
{

}

