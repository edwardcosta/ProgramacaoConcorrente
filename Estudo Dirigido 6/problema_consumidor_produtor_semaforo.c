/******************************************
* Aluno: Eduardo Calandrini Rocha da Costa
* Matricula: 13/0059595
* Programacao Concorrente - Prof Alchieri
*******************************************/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define N 10

int b[N];
int cont = 0;

pthread_cond_t prod_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cons_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mp = PTHREAD_MUTEX_INITIALIZER;

int prod_pos = 0;
int cons_pos = 0;

void * produtor(void * arg){
  while(1){
    printf("Proutor - Vai produzir\n");
    int p = (int)(drand48()*1000);
    pthread_mutex_lock(&mp);
    while (cont == N) {
      /* code */
      printf("Produtor - Dormindo\n");
      pthread_cond_wait(&prod_cond,&mp);
    }
    b[prod_pos]=p;
    prod_pos = (prod_pos+1)%N;
    printf("Produtor - produzindo\n");
    sleep(2);
    printf("Produtor - Produziu\n");
    cont++;
    if(cont == 1){
      pthread_cond_signal(&cons_cond);
    }
    pthread_mutex_unlock(&mp);
  }
}

void * consumidor(void * arg){
  int id = (int) arg;
  while(1){
    printf("Consumidor %d - Quer Consumir\n", id);
    pthread_mutex_lock(&mp);
    while (cont == 0) {
      /* code */
      printf("Consumidor %d - Esperando\n",id);
      pthread_cond_wait(&cons_cond,&mp);
    }
    int c = b[cons_pos];
    cons_pos = (cons_pos+1)%N;
    cont--;
    if(cont == (N-1)){
      pthread_cond_signal(&prod_cond);
    }
    pthread_mutex_unlock(&mp);
    printf("Consumidor %d - Consumindo\n",id);
    sleep(5);
    printf("Consumidor %d - Consumiu\n",id );
  }
}

int main() {
  /* code */
  pthread_t c[N];
  int i;
  int *id;
  srand48(time(NULL));
  for(i = 0; i < N; i++){
    id = (int*)malloc(sizeof(int));
    *id = i;
    pthread_create(&c[i],NULL,consumidor,(void*)(id));
  }
  pthread_join(c[0],NULL);
  return 0;
}
