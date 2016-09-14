#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_USERS 30

sem_t vagas;
sem_t cheia;
sem_t mutex;

int c = 0;

void * usuario(void * arg){
    while (1) {
        /*escreve*/
        sem_wait(&vagas);
        sem_wait(&mutex);
        c++;
        if(c == NUM_USERS){
            sem_post(&cheia);
        }
        sem_post(&mutex);
    }
}

void *  pombo(){
    int i;
    while (1) {
        sem_wait(&cheia);
        sem_wait(&mutex);
        /*envia cartas*/
        c = 0;

        for(i = 0; i < NUM_USERS; i++){
            sem_post(&vagas);
        }
        /*retornar para pegar mis cartas*/
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
