/******************************************
* Aluno: Eduardo Calandrini Rocha da Costa
* Matricula: 13/0059595
* Programacao Concorrente - Prof Alchieri
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_LEITOR 4
#define NUM_ESCRITOR 2

pthread_mutex_t lock_bd = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_cont = PTHREAD_MUTEX_INITIALIZER;
int leitores_cont = 0;

void * leitores (void * arg){
    int i = *((int*)arg);
    while(1){
        printf("Leitor %d - Vai ler dados\n",i);
        pthread_mutex_lock(&lock_cont);
        leitores_cont++;
        if(leitores_cont == 1){
            pthread_mutex_lock(&lock_bd);
        }
        pthread_mutex_unlock(&lock_cont);
        /*LER DADOS*/
        printf("Leitor %d - Lendo dados\n",i);
        sleep(10);
        pthread_mutex_lock(&lock_cont);
        leitores_cont--;
        if(leitores_cont == 0){
            pthread_mutex_unlock(&lock_bd);
        }
        pthread_mutex_unlock(&lock_cont);
        /*PROCESSA DADOS LIDOS*/
        printf("Leitor %d - Processando dados lidos\n",i);
        sleep(2);
        printf("Leitor %d - Dados processados\n",i);
    }
}

void * escritores(void * arg){
    int i = *((int*)arg);
    while(1){
        /*PRODUZIR DADOS*/
        printf("Escritor %d - Produzindo dados\n",i);
        sleep(5);
        printf("Escritor %d - Vai escrever dados\n",i);
        pthread_mutex_lock(&lock_bd);
        /*ESCREVE DADOS*/
        printf("Escritor %d - Escrevendo dados\n",i);
        sleep(10);
        pthread_mutex_unlock(&lock_bd);
        printf("Escritor %d - Dados escritos\n",i);
    }
}

int main(){
    pthread_t l[NUM_LEITOR];
    pthread_t e[NUM_ESCRITOR];
    int i,j;
    int *idL,*idE;
    for(i = 0; i < NUM_ESCRITOR; i++){
        idE = (int*) malloc(sizeof(int));
        *idE = i;
        pthread_create(&e[i], NULL, escritores, (void *) (idE));
    }
    for(j = 0; j < NUM_LEITOR; j++){
        idL = (int*) malloc(sizeof(int));
        *idL = j;
        pthread_create(&l[j], NULL, leitores, (void *) (idL));
    }
    pthread_join(e[0],NULL);
    pthread_join(l[0],NULL);
    return 0;
}
