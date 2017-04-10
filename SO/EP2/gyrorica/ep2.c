/* ///////////////////////////////////////////////////// */
/*                        MAC0422                        */
/*                 Daniel Macêdo Batista                 */
/*                                                       */
/*                        ep2.c                          */
/*                                                       */
/*     Luiz Felipe Moumdjian Girotto   NUSP: 8941189     */
/*         Ricardo Lira da Fonseca   NUSP: 8536131       */
/*                                                       */
/* ///////////////////////////////////////////////////// */

/* //////////////////////// */
/* ///// BIBLIOTECAS ////// */
/* //////////////////////// */

#define _GNU_SOURCE

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

typedef struct pos
{
    /* Posições vazias na pista serão representadas por "-1" */
    int pistas[2];
    pthread_mutex_t position_mutex;

} position;

/* ////////////////////////////// */
/* ///// VARIAVEIS GLOBAIS ////// */
/* ////////////////////////////// */

int d, n; /* Argumentos de entrada */
position *velodromo;
int DEBUG = 0;
/* c_r */int ciclistas_ativos; /* Ciclistas (threads) ainda realizando a corrida */
/* c_r */int ciclistas_time[2]; /* Numero de ciclistas em cada time */
/* Os tres primeiros ciclistas de cada time por lap. */
/* Para achar quais são os do time t, no lap n, examine first_three[t][n*3, (n*3)+1, (n*3)+2]*/
/* c_3f */int first_three[2][48];
/* Vetor das colocações finais de cada time */
/* c_eor */int **finishing_places;
/* Vetor dos tempos finais de cada time */
/* c_eor */double **finishing_times;
/* c_b */int ciclistas_quebrados[2][3]; /* Vetor com as identificações dos corredores quebrados */
/* c_b */int ciclistas_quebrados_lap[2][3]; /* Vetor com os laps dos corredores quebrados */
/* Podemos saber o numero dos corredores quebrados, comparando os ciclistas de cada time, com n/2 */
/* b_nt */int barreira_esperados; /* Numero de threads esperadas pela barreira */
/* c_b_l */int turnos_quebrados[3]; /* Turnos em que houveram quebra de ciclistas */
/* winner */int finished_race = -1; /* If race has finished. Keeps winner team identifier */










/* Variaveis para o funcionamento do modo "v". Algumas coisas daqui podem ser reaproveitadas para acima */

int chegou_30[2]; /* Demonstra que chegou um ciclista, naquela volta, que decidiu reduzir sua velocidade para 30km/h */
int geral_lap[2]; /* Identifica se cada time INTEIRO terminou um lap, se o valor para geral_lap[time] == ciclistas_time[time] */

clock_t clock_inicial;
double elapsed;

char mode; /* Modo de execução ('u' ou 'v') */

/* Precisa inicializar os mutexes.
Inicializa com: e.g.: pthread_mutex_init(&pista0_mutex, NULL)
Bloqueia/Espera: e.g.: pthread_mutex_lock(&pista0_mutex)
Desbloqueia: e.g.: pthread_mutex_unlock(&pista0_mutex)
O lock trava o mutex caso este esteja destravado, e faz a thread esperar caso */
/* a thread ja esteja travada por outra */
pthread_mutex_t pista0_mutex;
pthread_mutex_t pista1_mutex;

pthread_mutex_t first_three_mutex;
pthread_mutex_t speed_mutex;
pthread_mutex_t break_mutex;
pthread_mutex_t aux_mutex;


/*b0*/pthread_barrier_t movement_barrier;
/*b1*/pthread_barrier_t begin_confirm_barrier;
/*b_aux*/pthread_barrier_t adjust_barrier;

/* pthread_barrier_t, para criar barreiras de sincronização */

/* /////////////////////// */
/* ///// PROTOTIPOS ////// */
/* /////////////////////// */

void *ciclista (void *args);

/* ///////////////// */
/* ///// MAIN ////// */
/* ///////////////// */

int main (int argc, char *argv[]) {

    pthread_t *ciclistas;

    int aux = 0;
    int *id_positions;

    d = atoi(argv[1]);
    n = atoi(argv[2]);
    /* Pois, em verdade, são n ciclistas por equipe, logo*/
    /* o número real de ciclistas é 2 * n */
    n = n * 2;
    mode = argv[3][0];

    if (argc == 5)
        if (argv[4][0] == 'd')
            DEBUG = 1;

    velodromo = malloc (d * sizeof(position));
    for (aux = 0; aux < d; aux++) {
        velodromo[aux].pistas[0] = -1;
        velodromo[aux].pistas[1] = -1;
        pthread_mutex_init(&(velodromo[aux].position_mutex), NULL);
    }

    pthread_mutex_init(&aux_mutex, NULL);
    pthread_mutex_init(&break_mutex, NULL);
    pthread_mutex_init(&first_three_mutex, NULL);
    pthread_mutex_init(&speed_mutex, NULL);

    ciclistas = malloc(n * sizeof(pthread_t));
    ciclistas_ativos = n;

    for (aux = 0; aux < 3; aux++)
        turnos_quebrados[aux] = 0;

    ciclistas_time[0] = n/2;
    ciclistas_time[1] = n/2;

    for (aux = 0; aux < 48; aux++) {
        first_three[0][aux] = -1;
        first_three[1][aux] = -1;
    }

    finishing_places =  malloc(2 * sizeof(int*));
    finishing_places[0] = malloc ((n/2) * sizeof(int));
    finishing_places[1] = malloc ((n/2) * sizeof(int));

    for (aux = 0; aux < n/2; aux++) {
        finishing_places[0][aux] = -1;
        finishing_places[1][aux] = -1;
    }

    finishing_times =  malloc(2 * sizeof(double*));
    finishing_times[0] = malloc ((n/2) * sizeof(double));
    finishing_times[1] = malloc ((n/2) * sizeof(double));

    for (aux = 0; aux < n/2; aux++) {
        finishing_times[0][aux] = -1;
        finishing_times[1][aux] = -1;
    }

    chegou_30[0] = 0;
    chegou_30[1] = 0;

    geral_lap[0] = 0;
    geral_lap[1] = 0;

    for (aux = 0; aux < 3; aux++) {
        ciclistas_quebrados[0][aux] = -1;
        ciclistas_quebrados[1][aux] = -1;
        ciclistas_quebrados_lap[0][aux] = -1;
        ciclistas_quebrados_lap[1][aux] = -1;
    }

    /* O Rodrigo faz n+1, pois a main é seu coordenador. */
    /* Quando quebra um ciclista, a barreira ficaria esperando */
    /* por algo que não mais existe. O Rodrigo possui quatro barreiras por conta disso. */
    /* Duas são auxiliares, e duas são principais. Quando um ciclista quebra, ele muda todos */
    /* para uma nova barreira com n-1 ciclistas, e as outras ele usa para fazer isso também. */
    pthread_barrier_init(&movement_barrier, NULL, n);
    /* A main precisa esperar estes ajustes também, se não ela atrapalhará */
    /* na reestruturação das duas barreiras principais.                    */
    pthread_barrier_init(&adjust_barrier, NULL, n + 1);
    barreira_esperados = n + 1;
    pthread_barrier_init(&begin_confirm_barrier, NULL, n + 1);

    if (argc > 4)
        if (strcmp(argv[4], "-d") == 0)
            DEBUG = 1;

    /* No caso, este id_positions é necessário por motivos de, se fizessemos */
    /* apenas com um "i", poderiamos criar threads mais rapido do que atualizarmos */
    /* o "i", e isto nos geraria problemas. */
    id_positions = malloc(n * sizeof(int));
    for (aux = 0; aux < n; aux++) {
        id_positions[aux] = aux;
        pthread_create(&(ciclistas[aux]), NULL, ciclista, &id_positions[aux]);
    }

    clock_inicial = clock();

    /* ////// LOOP PRINCIPAL ////// */
    while (ciclistas_ativos > 0) {
        pthread_barrier_wait(&adjust_barrier);

        if (mode == 'v') {
            if (geral_lap[0] == ciclistas_time[0]) {
                pthread_mutex_lock(&speed_mutex);
                printf("\n\n\nTodos terminaram um lap, no time 0\n");
                chegou_30[0] = 0;
                geral_lap[0] = 0;
                pthread_mutex_unlock(&speed_mutex);
            }
            if (geral_lap[1] == ciclistas_time[1]) {
                pthread_mutex_lock(&speed_mutex);
                printf("\n\n\nTodos terminaram um lap, no time 1\n");
                chegou_30[1] = 0;
                geral_lap[1] = 0;
                pthread_mutex_unlock(&speed_mutex);
            }
        }

        if (ciclistas_ativos < barreira_esperados - 1) {
            pthread_barrier_destroy(&adjust_barrier);
            pthread_barrier_init(&adjust_barrier, NULL, ciclistas_ativos + 1);
            barreira_esperados = ciclistas_ativos + 1;
        }

        if (DEBUG) {
            for (aux = 0; aux < d; aux++) {
                printf("[%d][%d] ", velodromo[aux].pistas[0], velodromo[aux].pistas[1]);
            }
            printf("\n");
        }

        pthread_barrier_wait(&begin_confirm_barrier);
        
    }

    free (id_positions);

    /* Vai precisar disto?*/
    /*for (aux = 0; aux < n; aux++){
        pthread_join(ciclistas[aux], NULL);
    }*/

    printf("\n\nRelatório final:\n");
    printf("Equipe vencedora: %d\n", finished_race);
    printf("\nColocacoes da equipe %d:\n", finished_race);
    for (aux = 0; aux < ciclistas_time[finished_race]; aux++) {
        printf("%d terminou a corrida no tempo %f\n", finishing_places[finished_race][aux], finishing_times[finished_race][aux]);
        /* Aqui vem os prints do tempo, que vem ao lado de cada id do ciclista */
    }
    if (finished_race == 0) {
        printf("\nColocacoes da equipe 1:\n");
        for (aux = 0; aux < ciclistas_time[1]; aux++) {
            printf("%d terminou a corrida no tempo %f\n", finishing_places[1][aux], finishing_times[1][aux]);
            /* Aqui vem os prints do tempo, que vem ao lado de cada id do ciclista */
        }
    }
    if (finished_race == 1) {
        printf("\nColocacoes da equipe 0:\n");
        for (aux = 0; aux < ciclistas_time[0]; aux++) {
            printf("%d terminou a corrida no tempo %f\n", finishing_places[0][aux], finishing_times[0][aux]);
            /* Aqui vem os prints do tempo, que vem ao lado de cada id do ciclista */
        }
    }

    printf("\nCiclistas quebrados:\n");
    for (aux = 0; aux < n/2 - ciclistas_time[0]; aux++)
        printf("%d, quebrou na volta %d\n", ciclistas_quebrados[0][aux], ciclistas_quebrados_lap[0][aux]);
    for (aux = 0; aux < n/2 - ciclistas_time[1]; aux++)
        printf("%d, quebrou na volta %d\n", ciclistas_quebrados[1][aux], ciclistas_quebrados_lap[1][aux]);

    printf("\n");


    free(ciclistas);
    free(velodromo);
    free(finishing_places[0]);
    free(finishing_places[1]);
    free(finishing_places);
    free(finishing_times[0]);
    free(finishing_times[1]);
    free(finishing_times);

    
    /* Não precisa destruir a movement_barrier, por que na ultima iteração, */
    /* tentaremos inicializar a barreira com 0 ciclistas, e isto não terá sucesso */
    pthread_barrier_destroy(&begin_confirm_barrier);
    pthread_barrier_destroy(&adjust_barrier);
    

    return 0;
}

/* /////////////////////////////// */
/* ///// FUNCAO DE CICLISTA ////// */
/* /////////////////////////////// */

void *ciclista (void *args) {
    /* Será passado como argumento o id de cada ciclista. */

    int id; /* Identificação do corredor. Passado como argumento. */
    int pos; /* Posição do ciclista na pista */
    int start; /* Posição inicial do ciclista */
    int team; /* Time do corredor. Determinado pela paridade de sua identificação */
    int vel; /* Velocidade do ciclista. 1 = 30km/h, 2 = 60km/h*/
    int lap = 0; /* Quantas voltas este ciclista deu */
    int pista = 0; /* Em que pista o ciclista se localiza no momento */
    int move_counter = 0; /* Tem que checar se move_counter é maior ou igual a 2, e então ele pode se mover */
    int try_break = 0; /* Se o ciclista passou pela chance de quebrar */
    int broken = 0; /* Se o ciclista quebrou na corrida */

    int next_pos; /* Proxima posição para o ciclista */
    double random;
    int aux = 0;

    id = *((int*) args);

    /* Definindo velocidade inicial */
    if (mode == 'u') {
        vel = 2;
    }

    if (mode == 'v') {
        /* Todos começam a 30km/h no modo 'v', e depois do primeiro lap começam a variar */
        vel = 1;
    }

    /* Definindo posição inicial e equipe */

    if (id % 2 == 0) {
        /* Equipe 0 */
        team = 0;
        pos = 0;
        start = 0;
    }
    else {
        /* Equipe 1 */
        team = 1;
        pos = d/2;
        start = d/2;
    }

    /* //////////////////////////// */
    /* INICIO DO CICLO DE MOVIMENTO */
    /* //////////////////////////// */

    /* TESTE, DEPOIS MUDAR PARA 16*/
    while (lap < 16) {

        pthread_barrier_wait(&adjust_barrier);
        pthread_barrier_wait(&begin_confirm_barrier);

        /* Condições para a quebra */
        if (lap > 0) {
            if (lap % 4 == 0)  {
                if (try_break == 0) {
                    /* Precisa começar a guardar quantos tem por time, e decrementar quando quebra, e limitar quando tem 3 (no if acima) */
                    random = rand() % 10;
                    pthread_mutex_lock(&break_mutex);
                    if (ciclistas_time[team] > 3 && turnos_quebrados[(lap/4) - 1] == 0 && random < 1) {
                        printf("\n//////////////////////////////////////////////////////////////////////\n");
                        printf("Quebrou o ciclista %d, na volta %d, que estava na posição %d!\n", id, lap, pos);
                        printf("//////////////////////////////////////////////////////////////////////\n");
                        /* Precisamos guardar as informações de cada um que quebrar */
                        turnos_quebrados[(lap/4) - 1] = 1;
                        ciclistas_time[team]--;
                        broken = 1;
                        pthread_mutex_unlock(&break_mutex);

                        pthread_barrier_wait(&movement_barrier);
                        break;
                    }
                    pthread_mutex_unlock(&break_mutex);
                    try_break = 1;
                    /* Coloca o random aqui, e ter um vetor global de 3 espaços (quebras no lap 4, 8 e 12), que identifica se alguem quebrou */
                    /* nos ditos laps, e, caso sim, não permitir que mais ninguem quebre nestes laps. Também ter uma flag que informa se o   */
                    /* ciclista saiu do loop por quebra, e não por fim de corrida, e lidar com as condições abaixo do loop de acordo.        */
                }
            }

            else {
                try_break = 0;
            }
        }

        if (move_counter >= 2) {
            /* Inicio de seção crítica */
            next_pos = (pos + 1) % d;
            pthread_mutex_lock(&(velodromo[next_pos].position_mutex));
            if (velodromo[next_pos].pistas[0] == -1) {
                /* Consegue andar na pista 0 */
                velodromo[next_pos].pistas[0] = id;
                velodromo[pos].pistas[pista] = -1;
                pista = 0;
                pos = next_pos;
                if (pos == start) {
                    pthread_mutex_lock(&first_three_mutex);
                    if (mode == 'v') {
                        if (lap != 0) {
                            pthread_mutex_lock(&speed_mutex);
                            geral_lap[team]++;

                            if (chegou_30[team] == 0) {
                                if ((rand() % 2) < 1) {
                                    printf("O ciclista %d decidiu ficar na velocidade 30.\n", id);
                                    vel = 1;
                                    chegou_30[team] = 1;
                                }
                                else {
                                    printf("O ciclista %d decidiu ficar na velocidade 60.\n", id);
                                    vel  = 2;
                                }
                            }
                            else {
                                printf("O ciclista %d FOI OBRIGADO a ficar na velocidade 30.\n", id);
                                vel = 1;
                            }
                            pthread_mutex_unlock(&speed_mutex);
                        }
                    }

                    for (aux = 0; aux < 3; aux++) {
                        if (first_three[team][(lap * 3) + aux] == -1) {
                            first_three[team][(lap * 3) + aux] = id;
                            if (aux == 2) {
                                printf("\n Os tres primeiros a realizar o lap, na equipe %d, foram os ciclistas: %d, %d e %d.\n", team, first_three[team][(lap * 3)], first_three[team][(lap * 3)+ 1], id);
                                printf("Estamos, neste momento, na volta %d.\n", lap);
                                printf("O terceiro ciclista chegou no tempo %f\n", ((double)(clock()-clock_inicial))/((double)CLOCKS_PER_SEC));
                            }
                            break;
                        }
                    }
                    pthread_mutex_unlock(&first_three_mutex);
                    lap += 1;
                }
                pthread_mutex_unlock(&(velodromo[pos].position_mutex));
            }
            else if (velodromo[next_pos].pistas[1] == -1) {
                /* Consegue andar na pista 1 */
                velodromo[next_pos].pistas[1] = id;
                velodromo[pos].pistas[pista] = -1;
                pos = next_pos;
                pista = 1;
                if (pos == start) {
                    pthread_mutex_lock(&first_three_mutex);
                    if (mode == 'v') {
                        if (lap != 0) {
                            pthread_mutex_lock(&speed_mutex);
                            geral_lap[team]++;

                            if (chegou_30[team] == 0) {
                                if ((rand() % 2) < 1) {
                                    printf("O ciclista %d decidiu ficar na velocidade 30.\n", id);
                                    vel = 1;
                                    chegou_30[team] = 1;
                                }
                                else {
                                    printf("O ciclista %d decidiu ficar na velocidade 60.\n", id);
                                    vel  = 2;
                                }
                            }
                            else {
                                printf("O ciclista %d FOI OBRIGADO a ficar na velocidade 30.\n", id);
                                vel = 1;
                            }
                            pthread_mutex_unlock(&speed_mutex);
                        }
                    }

                    for (aux = 0; aux < 3; aux++) {
                        if (first_three[team][(lap * 3) + aux] == -1) {
                            first_three[team][(lap * 3) + aux] = id;
                            if (aux == 2) {
                                printf("\n Os tres primeiros a realizar o lap, na equipe %d, foram os ciclistas: %d, %d e %d.\n", team, first_three[team][(lap * 3)], first_three[team][(lap * 3)+ 1], id);
                                printf("A volta completa pelo time, neste momento, foi a volta %d.\n", lap);
                                printf("O terceiro ciclista chegou no tempo %f\n", ((double)(clock()-clock_inicial))/((double)CLOCKS_PER_SEC));
                            }
                            break;
                        }
                    }
                    pthread_mutex_unlock(&first_three_mutex);
                    lap += 1;
                }
                pthread_mutex_unlock(&(velodromo[pos].position_mutex));
            }
            else {
                /* Todas as pistas estão bloqueadas */
                pthread_mutex_unlock(&(velodromo[next_pos].position_mutex));
            }

            /* É justo/realista que esteja assim?*/
            move_counter = 0;

        }
        else {
            move_counter += vel;
        }
        pthread_barrier_wait(&movement_barrier);
    }


    pthread_mutex_lock(&aux_mutex);
    ciclistas_ativos--;
    velodromo[pos].pistas[pista] = -1;

    if (broken == 0) {

        if (finished_race == -1)
            finished_race = team;

        for (aux = 0; aux < ciclistas_time[team]; aux++) {
            if (finishing_places[team][aux] == -1) {
                finishing_places[team][aux] = id;
                finishing_times[team][aux] = ((double)(clock()-clock_inicial))/((double)CLOCKS_PER_SEC);
                break;
            }
        }
    }

    else {
        for (aux = 0; aux < 3; aux++) {
            if (ciclistas_quebrados[team][aux] == -1) {
                ciclistas_quebrados[team][aux] = id;
                ciclistas_quebrados_lap[team][aux] = lap;
                break;
            }
        }
    }

    pthread_barrier_destroy(&movement_barrier);
    pthread_barrier_init(&movement_barrier, NULL, ciclistas_ativos);
    pthread_barrier_destroy(&begin_confirm_barrier);
    pthread_barrier_init(&begin_confirm_barrier, NULL, ciclistas_ativos + 1);
    pthread_mutex_unlock(&aux_mutex);

    pthread_barrier_wait(&adjust_barrier);

    return NULL;

}
