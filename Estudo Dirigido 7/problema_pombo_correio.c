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

#define NUM_USERS 30

sem_t vagas;
sem_t cheia;int i;
sem_t mutex;

int c = 0;

void * usuario(void * arg){
    int i = *((int *) arg);
    while (1) {
        /*escreve*/
        printf("Usuario %d escrevendo\n", i);
        sleep(5);
        printf("Usuario %d quer postar a carta\n",i);
        sem_wait(&vagas);
        sem_wait(&mutex);
        c++;
        printf("Usuario %d postou a carta\n", i);
        if(c == NUM_USERS){
            sem_post(&cheia);
        }
        sem_post(&mutex);
    }
}

void *  pombo(){
    while (1) {
        sem_wait(&cheia);
        sem_wait(&mutex);
        /*envia cartas*/
        printf("Bolsa cheia, enviando cartas\n");
        sleep(1);

        c = 0;

        for(i = 0; i < NUM_USERS; i++){
            sem_post(&vagas);
        }
        printf("Pombo voando\n");
        sleep(2);
        /*retornar para pegar mis cartas*/
        printf("Cartas entregues, pombo retornando\n");
        sleep(2);
        sem_post(&mutex);
    }
}

int main(int argc, char const *argv[]) {
    /* code */
    sem_init(&vagas, 0, NUM_USERS);
    sem_init(&cheia, 0, 0);
    sem_init(&mutex, 0, 1);
    pthread_t users[NUM_USERS];
    pthread_t p;

    int i;
    int *id;

    pthread_create(&p, NULL, pombo, NULL);

    for(i = 0; i < NUM_USERS; i++){
        id = (int*) malloc(sizeof(int));
        *id = i;
        pthread_create(&users[i], NULL, usuario, (void *) (id));
    }
    pthread_join(users[0],NULL);

    return 0;
}
