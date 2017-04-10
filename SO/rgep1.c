#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


/* //////////////////// */
/* ///// STRUCTS ////// */
/* //////////////////// */

typedef struct p {
	float t0;
	char name[100];
	float dt;
	float deadline;

	float decorrido;
	float tf;

	pthread_t id;

	int status; /*-1 se não tiver chegado ainda, 0 se tiver em standby, 1 se estiver rodando*/

	/*Debug variables*/
	int trace_line;

} processo;

typedef struct cel
{
	processo *proc;
	struct cel *prox;
} celula;

/* /////////////////////// */
/* ///// PROTOTIPOS ////// */
/* /////////////////////// */

celula* coletar(char *arquivo, celula *lista_proc);
celula* CriaCelula (float t0, char *name, float dt, float deadline, int id);
celula* CriaCelulaTerminada (float t0, char *name, float dt, float deadline, float decorrido, float tf);
celula* InsereOrdenadoT0 (celula *nova, celula *lista);
celula* InsereNoFim (celula *nova, celula *lista);
celula* RetornaPrimeiroComStatusZero (celula *lista);
celula* InsereOrdenadoTimeLeft (celula *nova, celula *lista);
celula* RemoveProcessoTerminado (celula *lista);
void ImprimeOutput (celula *lista, FILE *output);
void LiberaLista (celula *lista);
void ImprimeLista (celula *lista);
void ImprimeListaPlus (celula *lista);
void ImprimeListaStatus (celula *lista);
void FCFS();
void SRTN();
void ECMF();
void consomeTempo();
void *RodaProcesso(void *args);
void *RodaProcesso_Preempcao(void *args);

/* ////////////////////////////// */
/* ///// VARIAVEIS GLOBAIS ////// */
/* ////////////////////////////// */

int DEBUG;

int n_processos;
int count_processos;
pthread_t *glo;

int max_threads;
int n_threads; /* processos sendo utilizados no momento */

clock_t clock_total;
double elapsed;

int n_troca_de_contexto;

/* Semaforo para secoes criticas. */
sem_t s_mutex;

celula *lista_proc;
celula *lista_proc_ordenados;
celula *lista_proc_terminaram;

/* ///////////////// */
/* ///// MAIN ////// */
/* ///////////////// */

int main (int argc, char **argv) {
	/* printf("S K I L L E D R O Y Z\n"); */
	int escid, i;
	FILE *out;

	lista_proc = NULL;
	lista_proc_ordenados = NULL;
	lista_proc_terminaram = NULL;

	DEBUG = 0;
	n_processos = 0;
	count_processos = 0;

	n_troca_de_contexto = 0;

	max_threads = sysconf(_SC_NPROCESSORS_ONLN);
	n_threads = 0;

	escid = atoi(argv[1]);
	lista_proc = coletar(argv[2], lista_proc);
	out = fopen (argv[3], "w");

	if (argc == 5)
		if (argv[4][0] == 'd')
			DEBUG = 1;

	clock_total = clock();

	/* Inicializa o semáforo para todas as threads */
  	sem_unlink("s_mutex");
  	sem_init(&s_mutex, 0, 1);
	
	if (escid == 1) {
		/* First Come First Serve */
		printf("Começando First Come First Serve! \n\n");
		fflush(stdout);
		FCFS();
	}

	else if (escid == 2) {
		/*Shortest Remaining Time Next*/
		printf("Começando Shortest Remaining Time Next! \n\n");
		fflush(stdout);
		SRTN();
	}

	else if (escid == 3) {
		/*Escalonamento Com Múltiplas Filas*/
		printf("Começando Escalonamento Com Múltiplas Filas! \n\n");
		fflush(stdout);
		ECMF();
	}

	else {
		printf("Erro! ID do Escalonador fora da range reconhecivel.\n");
		fflush(stdout);
	}

	/*Espera todos processos terem acabado*/
	while (count_processos < n_processos) continue; /*Buga se não deixa nada, por alguma razão...*/


	/*Espera todas threads terem terminado*/
	for (i = 0; i < n_processos; i++){
		pthread_join(glo[i], NULL);
	}

	printf("Imprimindo lista\n");

	ImprimeListaPlus(lista_proc_terminaram);
	ImprimeOutput(lista_proc_terminaram, out);

	if (DEBUG) {
		fprintf (stderr, "\nNumero de trocas de contexto: %d\n", n_troca_de_contexto);
	}

	fclose(out);

	LiberaLista(lista_proc);
	LiberaLista(lista_proc_terminaram);
	/*ainda falta liberar coisas*/

	sem_destroy(&s_mutex);

	return 0;
}

/* /////////////////////////// */
/* ///// FUNCOES GERAIS ////// */
/* /////////////////////////// */

celula* coletar(char *arquivo, celula *lista_proc) {
	char tmp[100];
	FILE *trace;
	int i;
	double t0, dt, deadline;
	char name[100]; /*100 =  maximo nome do processo*/

	trace = fopen(arquivo, "r");

	/* Le o arquivo de entrada e conta o numero de processos */
	for (i=0;fgets(tmp, 100, trace) != NULL;++i);
	n_processos = i;

	glo = malloc(n_processos * sizeof(pthread_t));

	rewind(trace);

	/* Enfila os processos */
	for (i = 0; fscanf(trace, "%lf %s %lf %lf", &t0, name, &dt, &deadline) != EOF; ++i) {
		celula *c = CriaCelula (t0, name, dt, deadline, i);
		/*printf("%f %s %f %f\n", t0, name, dt, deadline);*/
		lista_proc = InsereOrdenadoT0 (c, lista_proc);
	}


	ImprimeLista(lista_proc);
	printf("\n\n");
	fclose(trace);

	return lista_proc;
}

void FCFS() {
	celula *c;
	c = lista_proc;
	while(c != NULL) {
		while(c->proc->t0 > elapsed)
			consomeTempo();

		if (DEBUG) {
			fprintf(stderr, "~~Chegou o processo de nome: %s. Sua linha no trace era: %d\n\n", c->proc->name, c->proc->trace_line);
		}

		while(n_threads >= max_threads)
			consomeTempo();

		/*Seção critica*/
		sem_wait(&s_mutex);

		n_threads++;
		pthread_create(&(c->proc->id), NULL, &RodaProcesso, (void*) c->proc);

		/*Fim da seção critica*/
 		sem_post(&s_mutex);

		c = c->prox;
	}
}

void SRTN() {
	celula *c, *temp;
	c = lista_proc;

	while(count_processos < n_processos) {
		if(c != NULL && c->proc->t0 < elapsed) {
			/*Seção critica*/
			sem_wait(&s_mutex);
			if (DEBUG) {
				fprintf(stderr, "~~Chegou o processo de nome: %s. Sua linha no trace era: %d\n\n", c->proc->name, c->proc->trace_line);
			}	
			lista_proc_ordenados = InsereOrdenadoTimeLeft(CriaCelula(c->proc->t0, c->proc->name, c->proc->dt, c->proc->deadline, c->proc->trace_line), lista_proc_ordenados);
			/*compara com todos processos na lista_proc_ordenados com status 1. Se bagulho do que chegou for mais rapido que o que ta comparanado, muda status do comparado para 0
			adiciona c->proc na lista de ordenados*/
			c = c->prox;
			/*Fim da seção critica*/
 			sem_post(&s_mutex);
		}
		else
			consomeTempo();

 		if (lista_proc_ordenados ==  NULL) {
 			continue;
 		}
		if(n_threads >= max_threads) {
			consomeTempo();
			continue;
		}

		/*Seção critica*/
		sem_wait(&s_mutex);
		temp = RetornaPrimeiroComStatusZero(lista_proc_ordenados);
		if (temp == NULL) {
			/*Fim da seção critica*/
 			sem_post(&s_mutex);
			continue;
		}
		/*printf("              ******INICIANDO THREAD NOVA: %s\n", temp->proc->name);
		printf("%f %s %f %f %f %f\n\n", temp->proc->t0, temp->proc->name, temp->proc->dt, temp->proc->deadline, temp->proc->decorrido, temp->proc->tf);*/
		n_threads++;
		temp->proc->status = 1;
		pthread_create(&(temp->proc->id), NULL, &RodaProcesso_Preempcao, (void*) temp->proc);

		/*Fim da seção critica*/
 		sem_post(&s_mutex);
	}
}

void ECMF() {

}

void consomeTempo() {
	int i;
	for (i = 0; i < 1000; i++);
	elapsed = ((double)(clock()-clock_total))/((double)CLOCKS_PER_SEC);
}

void *RodaProcesso_Preempcao(void *args) {
	int sair = 0;
	processo *p;
	clock_t i_clock;

	/* Comeca secao critica. */
	sem_wait(&s_mutex);

	p = (processo*) args;
	if (DEBUG) {
		fprintf(stderr, "++Processo de nome: %s está usando a CPU 1.\n\n", p->name);
	}

	/* Termina seção crítica. */
	sem_post(&s_mutex);

	/* Calcula o tempo real que o processo esta sendo processado */
	while (p->decorrido < p->dt) {
		i_clock = clock();
		p->decorrido += ((double)(clock()-i_clock))/((double)CLOCKS_PER_SEC);

		
		/*Seção crítica*/

		sem_wait(&s_mutex);

		if (p->status == 0) {
			sair = 1;
			--n_threads;
			n_troca_de_contexto++;
			if (DEBUG) {
				fprintf(stderr, "##O processo de nome: %s parou. Fazendo troca de contexto numero %d\n\n", p->name, n_troca_de_contexto);
			}
		}

		/*Fim da seção crítica*/
		sem_post(&s_mutex);
		
		if (sair)
			return NULL;
	}

	/*Seção crítica*/
	sem_wait(&s_mutex);

	/* Marca o tempo que o processo acabou e o tempo de duração */
	p->tf = ((double)(clock()-clock_total))/((double)CLOCKS_PER_SEC);
	p->status = 2; /*status de terminado*/

	--n_threads;

	lista_proc_terminaram = InsereNoFim(CriaCelulaTerminada (p->t0, p->name, p->dt, p->deadline, p->decorrido, p->tf), lista_proc_terminaram);
	if (DEBUG) {
		fprintf(stderr, "--Terminou o processo de nome: %s. Ele sera escrito na linha %d do output.\n\n", p->name, count_processos);
	}

	lista_proc_ordenados = RemoveProcessoTerminado(lista_proc_ordenados); /*cuida dos free da lista de ordenados*/


	count_processos++;
	/*Fim da seção crítica*/
	sem_post(&s_mutex);

	return NULL;
}

void *RodaProcesso(void *args) {
  processo *p;
  clock_t i_clock;

  /* Comeca secao critica. */
  sem_wait(&s_mutex);

  p = (processo*) args;
	if (DEBUG) {
		fprintf(stderr, "++Processo de nome: %s está usando a CPU 1.\n\n", p->name);
	}


  /* Termina seção crítica. */
  sem_post(&s_mutex);

  /* Calcula o tempo real que o processo esta sendo processado */
  while (p->decorrido < p->dt) {
    i_clock = clock();
    p->decorrido += ((double)(clock()-i_clock))/((double)CLOCKS_PER_SEC);
  }

  /*Seção crítica*/
  sem_wait(&s_mutex);

  /* Marca o tempo que o processo acabou e o tempo de duração */
  p->tf = ((double)(clock()-clock_total))/((double)CLOCKS_PER_SEC);


  --n_threads;

  lista_proc_terminaram = InsereNoFim(CriaCelulaTerminada (p->t0, p->name, p->dt, p->deadline, p->decorrido, p->tf), lista_proc_terminaram);
  if (DEBUG) {
  	fprintf(stderr, "--Terminou o processo de nome: %s. Ele sera escrito na linha %d do output.\n\n", p->name, count_processos);
  }
  count_processos++;

  /*Fim da seção crítica*/
  sem_post(&s_mutex);
  
  return NULL;
}


/* /////////////////////////////////////// */
/* ///// FUNCOES DE LISTA ENCADEADA ////// */
/* /////////////////////////////////////// */

celula* CriaCelula (float t0, char *name, float dt, float deadline, int id) {
	celula *c;
	processo *p;

	c = malloc (sizeof(celula));
	p = malloc (sizeof(processo));

	p->t0 = t0;
	strcpy(p->name, name);
	p->dt = dt;
	p->deadline = deadline;

	p->decorrido = 0;
	p->tf = 0;

	p->id = glo[id];
	p->status = -1;

	p->trace_line = id;

	c->proc = p;
	c->prox = NULL; /*será mudado depois*/

	return c;
}

celula* CriaCelulaTerminada (float t0, char *name, float dt, float deadline, float decorrido, float tf) {
	celula *c;
	processo *p;

	c = malloc (sizeof(celula));
	p = malloc (sizeof(processo));

	p->t0 = t0;
	strcpy(p->name, name);
	p->dt = dt;
	p->deadline = deadline;

	p->decorrido = decorrido;
	p->tf = tf;

	c->proc = p;
	c->prox = NULL; /*será mudado depois*/

	return c;
}

/* Insere na lista ordenando por t0 */
celula* InsereOrdenadoT0 (celula *nova, celula *lista) {
	celula *p, *q;
	
	/* Cabeça da lista */
	p = lista;

	if (p == NULL) {
		lista = nova;
	}

	else {

		q = lista->prox;

		while (q != NULL && q->proc->t0 >= p->proc->t0) {
			p = q;
			q = q->prox;
		}
		nova->prox = q;
		p->prox = nova;
	}

	return lista;
}

/* Insere na lista ordenando por tf */
celula* InsereNoFim (celula *nova, celula *lista) {
	celula *p, *q;
	
	/* Cabeça da lista */
	p = lista;

	if (p == NULL) {
		lista = nova;
	}

	else {

		q = lista->prox;

		while (q != NULL) {
			p = q;
			q = q->prox;
		}
		nova->prox = q;
		p->prox = nova;
	}

	return lista;
}


void LiberaLista (celula *lista) {
	celula *p, *q;
	p = lista;
	if (p != NULL) {
		q = lista->prox;
		while (q != NULL) {
			free (p->proc);
			free (p);
			p = q;
			q = q->prox;
		}
	}
}

void ImprimeLista (celula *lista) {
	celula *p;
	p = lista;
	if(p == NULL)
		printf("A lista esta vazia!!\n");
	while (p != NULL) {
		printf("%f %s %f %f\n", p->proc->t0, p->proc->name, p->proc->dt, p->proc->deadline);
		p = p->prox;
	}
	printf("\n");
}

void ImprimeListaPlus (celula *lista) {
	celula *p;
	p = lista;
	while (p != NULL) {
		printf("%f %s %f %f %f %f\n", p->proc->t0, p->proc->name, p->proc->dt, p->proc->deadline, p->proc->decorrido, p->proc->tf);
		p = p->prox;
	}
}

void ImprimeListaStatus (celula *lista) {
	celula *p;
	p = lista;
	if(p == NULL)
		printf("A lista esta vazia!!\n");
	while (p != NULL) {
		printf("%f %s %f %f %d\n", p->proc->t0, p->proc->name, p->proc->dt, p->proc->deadline, p->proc->status);
		p = p->prox;
	}
}

void ImprimeOutput (celula *lista, FILE *output) {
	celula *p;
	p = lista;
	while (p != NULL) {
		fprintf (output, "%s %f %f\n", p->proc->name, p->proc->tf, (p->proc->tf - p->proc->t0));
		p = p->prox;
	}
	fprintf (output, "Numero de trocas de contexto: %d\n", n_troca_de_contexto);
}

celula* InsereOrdenadoTimeLeft (celula *nova, celula *lista) {
	celula *p, *q;
	
	/* Cabeça da lista */
	p = lista;

	nova->proc->status = 0;

	if (p == NULL) {
		lista = nova;
	}

	else {

		q = lista->prox;

		while (q != NULL && (q->proc->dt - q->proc->decorrido) <= (nova->proc->dt - nova->proc->decorrido)) {
			p = q;
			q = q->prox;
		}
		if (q != NULL && q->proc->status == 1) {
			nova->prox = q;
			p->prox = nova;

			/* Percorre ate achar o mais lento dos rapidos */
			while (q != NULL && q->proc->status == 0) {
				p = q;
				q = q->prox;
			}

			if (q != NULL) {
				q->proc->status = 0;
			}
		}
		else if (q != NULL) {
			nova->prox = q;
			p->prox = nova;
		}
		else {
			nova->prox = NULL;
			p->prox = nova;
		}
	}

	return lista;
}

celula* RetornaPrimeiroComStatusZero (celula *lista) {
	celula *p;
	p = lista;
	while (p != NULL && p->proc->status != 0) {
		p = p->prox;
	}
	if (p != NULL) {
		if (p->proc->t0 != p->proc->t0 || strcmp(p->proc->name, "") == 0) {
			/*Case is a broken process*/
			return NULL;
		}
		return p;
	}
	else {
		return NULL;
	}
}

celula* RemoveProcessoTerminado (celula *lista) {
	celula *p, *q;
	p = lista_proc_ordenados;
	if (p != NULL) {
		if (p->proc->status == 2) {
			if (p->prox == NULL) {
				free(p->proc);
				free(p);
			}
			else {
				lista_proc_ordenados = lista_proc_ordenados->prox;
				free(p->proc);
				free(p);
			}
		}
		else {
			q = lista_proc_ordenados->prox;
			while (q != NULL && q->proc->status != 2) {
				p = q;
				q = q->prox;
			}
			if (q == NULL)
				printf("ERRO! Não encontrou nenhum processo terminado!\n");
			else {
				p->prox = q->prox;
				free(q->proc);
				free(q);
			}
		}
	}
	return lista_proc_ordenados;
}
