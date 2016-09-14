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

#define NUM_MACACOS 20

char atravessar_corda(int num, char monte);
char atravessar_a_pra_b (int num);
char atravessar_b_pra_a (int num);

pthread_mutex_t lock_atravessar = PTHREAD_MUTEX_INITIALIZER; /*lock para a acao de atravessar dos macacos*/
pthread_mutex_t lock_macacos_a_b_cont = PTHREAD_MUTEX_INITIALIZER;/*lock para o contador de macacos atravessando de a para b*/
pthread_mutex_t lock_macacos_b_a_cont = PTHREAD_MUTEX_INITIALIZER;/*lock para o contador de macacos atravessando de b para a*/
pthread_mutex_t lock_macacos_a_cont = PTHREAD_MUTEX_INITIALIZER;/*lock para o contador de macacos no monte a*/
pthread_mutex_t lock_macacos_b_cont = PTHREAD_MUTEX_INITIALIZER;/*lock para o contador de macacos no monte b*/

int macacos_a_cont = 0;
int macacos_b_cont = 0;
int macacos_a_b_cont = 0;
int macacos_b_a_cont = 0;

void * macaco(void * arg){
    char monte = 'a';/*char que define qual monte o macaco esta*/
    int num = *((int*)arg);
    int atravessar;/*variavel que define se o macaco vai querer atravessar*/
    while(1){
        atravessar = rand()%10;/*30% de chance do macaco atravessar*/
        if(atravessar<2){
            if(monte == 'a'){/*verificacao em qual monte o macaco esta*/
                monte = atravessar_a_pra_b(num);/*se estiver no monte a ele atravessa no sentido a b*/
            }else{
                monte = atravessar_b_pra_a(num);/*se estiver no monte b ele atravessa no sentido b a*/
            }
        }else{
            sleep(10);/*se o macaco nao quiser atravessar ele espera*/
        }
    }
}

/*sentido a b*/
char atravessar_a_pra_b (int num){
    printf("Macaco %d - Quer ir de A para B\n",num);
    pthread_mutex_lock(&lock_macacos_a_b_cont);
    macacos_a_b_cont++;
    if(macacos_a_b_cont == 1){
        pthread_mutex_lock(&lock_atravessar);
    }
    pthread_mutex_lock(&lock_macacos_a_cont);
    macacos_a_cont--;
    pthread_mutex_unlock(&lock_macacos_a_b_cont);
    pthread_mutex_unlock(&lock_macacos_a_cont);
    /*ATRAVESSAR MACACOS*/
    printf("Macaco %d - Atravessando de A para B\n",num);
    sleep(5);
    pthread_mutex_lock(&lock_macacos_a_b_cont);
    macacos_a_b_cont--;
    printf("Macacos - Número de macacos atravessando de A para B %d\n",macacos_a_b_cont);
    if(macacos_a_b_cont == 0){
        pthread_mutex_unlock(&lock_atravessar);
    }
    pthread_mutex_lock(&lock_macacos_b_cont);
    macacos_b_cont++;
    printf("Macacos - Número de macacos em A %d\n",macacos_a_cont);
    printf("Macacos - Número de macacos em B %d\n",macacos_b_cont);
    pthread_mutex_unlock(&lock_macacos_a_b_cont);
    pthread_mutex_unlock(&lock_macacos_b_cont);
    printf("Macaco %d - Atravessou de A para B\n",num);
    return 'b';
}

char atravessar_b_pra_a (int num){
    printf("Macaco %d - Quer ir de B para A\n",num);
    pthread_mutex_lock(&lock_macacos_b_a_cont);
    macacos_b_a_cont++;
    if(macacos_b_a_cont == 1){
        pthread_mutex_lock(&lock_atravessar);
    }
    pthread_mutex_lock(&lock_macacos_b_cont);
    macacos_b_cont--;
    pthread_mutex_unlock(&lock_macacos_b_a_cont);
    pthread_mutex_unlock(&lock_macacos_b_cont);
    /*ATRAVESSAR MACACOS*/
    printf("Macaco %d - Atravessando de B para A\n",num);
    sleep(5);
    pthread_mutex_lock(&lock_macacos_b_a_cont);
    macacos_b_a_cont--;
    printf("Macacos - Número de macacos atravessando de B para A %d\n",macacos_b_a_cont);
    if(macacos_b_a_cont == 0){
        pthread_mutex_unlock(&lock_atravessar);
    }
    pthread_mutex_lock(&lock_macacos_a_cont);
    macacos_a_cont++;
    printf("Macacos - Número de macacos em A %d\n",macacos_a_cont);
    printf("Macacos - Número de macacos em B %d\n",macacos_b_cont);
    pthread_mutex_unlock(&lock_macacos_b_a_cont);
    pthread_mutex_unlock(&lock_macacos_a_cont);
    printf("Macaco %d - Atravessou de B para A\n",num);
    return 'a';
}

int main(){
    pthread_t m[NUM_MACACOS];
    int i;
    int *id;
    srand((unsigned)time(NULL));
    macacos_a_cont = (int)NUM_MACACOS;
    for(i = 0; i < NUM_MACACOS; i++){
        id = (int*) malloc(sizeof(int));
        *id = i;
        pthread_create(&m[i], NULL, macaco, (void *) (id));
    }
    pthread_join(m[0],NULL);
    return 0;
}
