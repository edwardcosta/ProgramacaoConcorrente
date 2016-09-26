/******************************************
 * Aluno: Eduardo Calandrini Rocha da Costa
 * Matricula: 13/0059595
 * Programacao Concorrente - Prof Alchieri
 *******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_CLI 10
#define CADEIRAS 5

int i;
int c = 0;

sem_t cadeira_b;
sem_t cadeiras;
sem_t cliente_cadeira;
sem_t cabelo_cortado;

void * cliente(void * arg){
    int i = *((int *) arg);
    printf("Cliente %d chegou\n",i );
    if(sem_trywait(&cadeiras)==0){
        printf("Cliente %d quer cortar o cabelo. Esperando.\n",i);
        sem_wait(&cadeira_b);
        printf("Cliente %d cortando o cabelo\n",i );
        sem_post(&cadeiras);
        sem_post(&cliente_cadeira);
        sem_wait(&cabelo_cortado);
        sem_post(&cadeira_b);
        printf("Cliente %d de cabelo cortado\n",i );
    }else{
        /* foi embora */
        printf("Cliente %d foi embora\n",i);
    }
    return NULL;
}

void * cabeleleiro(void * arg){
    while (1) {
        /* code */
        sem_wait(&cliente_cadeira);
        /* corta cabelo */
        sleep(5);
        sem_post(&cabelo_cortado);
    }
}

int main(){
    sem_init(&cadeira_b, 0, 1);
    sem_init(&cadeiras, 0, CADEIRAS);
    sem_init(&cliente_cadeira, 0, 0);
    sem_init(&cabelo_cortado, 0, 0);

    pthread_t clientes[NUM_CLI];
    pthread_t c;

    int i;
    int *id;

    pthread_create(&c, NULL, cabeleleiro, NULL);

    for(i = 0; i < NUM_CLI; i++){
        id = (int*) malloc(sizeof(int));
        *id = i;
        pthread_create(&clientes[i], NULL, cliente, (void *) (id));
    }
    pthread_join(clientes[0],NULL);

    return 0;
}
