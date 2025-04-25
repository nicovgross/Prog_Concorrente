#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

long long int N; //número de elementos
long long int iter = 1; //iteração atual da sequência
int count; //número de primos da sequência
pthread_mutex_t mutex; 

typedef struct {
    int id;
    int nthreads;
} t_Args;

//verifica se n é primo
int ehPrimo(long long int n) {
    int i;
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    for (i = 3; i < sqrt(n) + 1; i += 2) {
        if(n%i==0) { return 0; }
    }
    return 1;
}

void* Tarefa(void *args) {
    t_Args *arg = (t_Args*) args;
    for(int i=arg->id; i<=N; i+=arg->nthreads) {
        pthread_mutex_lock(&mutex);
        if(ehPrimo(iter)) { count++; }
        iter++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char *argv[]) {
    pthread_t *tid; //identificadores das threads no sistema
    t_Args *args;
    int nthreads; //número de threads

    //le e avalia os parametros de entrada
    if(argc<3) {
        printf("Digite: %s <número de elementos> <número de threads>\n", argv[0]);
        return 1;
    }
    N = atoll(argv[1]); //lê N
    nthreads = atoi(argv[2]); //lê o número de threads

    //aloca as estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    args = (t_Args*) malloc(sizeof(t_Args) * nthreads);
    if(tid==NULL || args==NULL) {puts("ERRO--malloc"); return 2;}

    //inicilaiza o mutex (lock de exclusao mutua)
    pthread_mutex_init(&mutex, NULL);

    //cria as threads
    for(long int i=0; i<nthreads; i++) {
        args->id = i;
        args->nthreads = nthreads;
        if (pthread_create(&tid[i], NULL, Tarefa, (void*) args)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    //espera todas as threads terminarem
    for (int i=0; i<nthreads; i++) {
        if (pthread_join(tid[i], NULL)) {
            printf("--ERRO: pthread_join() \n");
            exit(-1); 
        } 
    } 

    //finaliza o mutex
    pthread_mutex_destroy(&mutex);
   
    //printa resultado final
    printf("Número de primos até %lld: %d\n", N, count);

    return 0;
}