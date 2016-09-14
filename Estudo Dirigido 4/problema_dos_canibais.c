/******************************************
* Aluno: Eduardo Calandrini Rocha da Costa
* Matricula: 13/0059595
* Programacao Concorrente - Prof Alchieri
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_CANIBAIS 20

pthread_mutex_t lock_cozinhar = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_canibais = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_comida = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_cozinhar = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_canibais = PTHREAD_COND_INITIALIZER;

int comida = 0;

void * cozinheiro(void *arg){
    pthread_mutex_lock(&lock_comida)
    while(comida > 0){
        cond_wait(&cond_cozinhar,&lock_comida);
    }
    /*FAZER COMIDA*/
    sleep(10);
    comida = 10;
    cond_broadcast(&cond_canibais);
    pthread_mutex_unlock(&lock_comida);
}

void * canibais(void * arg){
    pthread_mutex_lock(&lock_comida);
    while(comida==0){
        cond_wait(&cond_canibais,&lock_comida);
    }
    comida--;
    if(comida==0){
        cond_signal(&cond_cozinhar);
    }
    /*VAI COMER*/
    pthread_mutex_unlock(&lock_comida);
}

int main(){
    pthread_t m[NUM_CANIBAIS];
    int i;
    int *id;
    srand((unsigned)time(NULL));
    for(i = 0; i < NUM_MACACOS; i++){
        id = (int*) malloc(sizeof(int));
        *id = i;
        pthread_create(&m[i], NULL, macaco, (void *) (id));
    }
    pthread_join(m[0],NULL);
    return 0;
}
