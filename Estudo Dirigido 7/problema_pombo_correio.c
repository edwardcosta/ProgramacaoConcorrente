#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 30

sem__t vagas  = N;
sem__t cheia = 0;
sem_t mutex = 1;
int c = 0;

void usuario(){
    while (1) {
        /*escreve*/
        sem_wait(&vagas);
        sem_wait(&mutex);
        c++;
        if(c == N){
            sem_post(&cheia);
        }
        sem_post(&mutex);
    }
}

void  pombo(){
    while (1) {
        sem_wait(&cheia);
        sem_wait(&mutex);
        /*envia cartas*/
        c = 0;

        for(i = 0; i < N; i++){
            sem_post(&vagas);
        }
        /*retornar para pegar mis cartas*/
        sem_post(&mutex);
    }
}
