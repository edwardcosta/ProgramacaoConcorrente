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

int i;
int c = 0;

pthread_cond_t  cab = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

sem_t cadeiras;
sem_t cadeiras;
sem_t cheia;

void * cliente(void * arg){

}

void * cabeleleiro(void * arg){
    while (1) {
        /* code */
        pthread_cond_wait(&cab, &mutex);
    }
}
