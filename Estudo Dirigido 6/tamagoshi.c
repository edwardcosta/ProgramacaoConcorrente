/**
 * UNIVERSIDADE DE BRASILIA
 * Programação Concorrente 2/2016
 * Douglas Shiro YOkoyama 13/0024902
 * Produtores e consumidores
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
// #include <semaphore.h>

#define PROD 10
#define CONS 10
#define N 10
#define TRUE 1
#define EAGAIN 1
#define EBUSY 1
/**
 * Funções semáforo
 */
typedef struct {
	int value;
	int n_wait;
	pthread_mutex_t lock;
	pthread_cond_t cond;
}sem_t;

int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_wait(sem_t *sem);
int sem_post(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_getvalue(sem_t *sem, int *sval);
int sem_destroy(sem_t *sem);

sem_t sem;

void* produtor(void *arg);
void* consumidor(void *arg);
int produce_item();
int remove_item();
void insert_item(int item);
void consume_item(int item);

double drand48(void);
void srand48(long int seedval);

int b[N];
int cont = 0;
int prodpos = 0;
int conspos = 0;

int main(){

	pthread_t p[PROD];
	pthread_t c[CONS];

	sem_init(&sem, 0, N);

	int i;
	int* id = NULL;

	srand48(time(NULL));

	for(i = 0; i < PROD; i++){
		id = (int*)malloc(sizeof(int));
		*id = i;
		pthread_create(&p[i], NULL, produtor, (void*)(id));
	}

	for(i = 0; i < CONS; i++){
		id = (int*)malloc(sizeof(int));
		*id = i;
		pthread_create(&c[i], NULL, consumidor, (void*)(id));
	}

	pthread_join(p[0], NULL);
	pthread_join(c[0], NULL);

	return 0;
}

void* produtor(void* arg){
	int i = *((int*)arg);
	int item;

	while(TRUE){
		sem_wait(&sem);
		item = produce_item();
		b[prodpos] = item;
		prodpos = (prodpos + 1)%N;
		sem_post(&sem);
		insert_item(item);
	}
}

void* consumidor(void* arg){
	int i = *((int*)arg);
	int item;

	while(TRUE){
		sem_wait(&sem);
		item = remove_item(conspos);
		conspos = (conspos + 1)%N;
		sem_post(&sem);
		consume_item(item);
	}
}

int produce_item(){
	printf("Gerando...\n");
	sleep(1);
return (int)(drand48()*1000);
}

int remove_item(int conspos){
    printf("removendo..\n");
	sleep(1);
return b[conspos];
}

void insert_item(int item){
    printf("Unidades produzidas: %d\n", item);
	sleep(1);
}

void consume_item(int item){
    printf("Consumindo %d unidades\n", item);
	sleep(1);
}


/**
 * Funções semaforo
 */
int sem_init(sem_t *sem, int pshared, unsigned int value){
	sem->value = value;
	sem->n_wait = 0;
	pthread_mutex_init(&sem->lock, NULL);
	pthread_cond_init(&sem->cond, NULL);
	return 0;
}

int sem_wait(sem_t *sem){
	pthread_mutex_lock(&sem->lock);
	if(sem->value > 0)
		sem->value--;
	else{
		sem->n_wait++;
		pthread_cond_wait(&sem->cond, &sem->lock);
	}
	pthread_mutex_unlock(&sem->lock);
	return 0;
}

int sem_post(sem_t *sem){
	pthread_mutex_lock(&sem->lock);
	if (sem->n_wait){
		sem->n_wait--;
		pthread_cond_signal(&sem->cond);
	}else
		sem->value++;
	pthread_mutex_unlock(&sem->lock);
	return 0;
}

int sem_trywait(sem_t *sem){
	int r;
	pthread_mutex_lock(&sem->lock);
	if(sem->value > 0){
		sem->value--;
		r = 0;
	}else
		r = EAGAIN;
	pthread_mutex_unlock(&sem->lock);
	return r;
}

int sem_getvalue(sem_t *sem, int *sval){
	pthread_mutex_lock(&sem->lock);
	*sval = sem->value;
	pthread_mutex_unlock(&sem->lock);
	return 0;
}

int sem_destroy(sem_t *sem){
	if(sem->n_wait)
		return EBUSY;
	pthread_mutex_destroy(&sem->lock);
	pthread_cond_destroy(&sem->cond);
	return 0;
}
