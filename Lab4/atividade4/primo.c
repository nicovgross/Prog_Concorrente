#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
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
    long long int inicio = (N * arg->id) / arg->nthreads;
    long long int fim = (N * (arg->id+1)) / arg->nthreads;
    int local_count = 0;

    for(long long int i = inicio; i < fim; i++) {
        if (ehPrimo(i)) local_count++;
    }

    // usar mutex apenas para somar no total global
    pthread_mutex_lock(&mutex);
    count += local_count;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t *tid; //identificadores das threads no sistema
    t_Args *args;
    int nthreads; //número de threads
    struct timespec inicio, fim;
    double temp_exec, media;
    FILE *f;

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

    for(int i=0; i<5; i++) { 
        count=0;
        clock_gettime(CLOCK_MONOTONIC, &inicio);
        //inicilaiza o mutex (lock de exclusao mutua)
        pthread_mutex_init(&mutex, NULL);

        //cria as threads
        for(long int i=0; i<nthreads; i++) {
            args[i].id = i;
            args[i].nthreads = nthreads;
            if (pthread_create(&tid[i], NULL, Tarefa, (void*) &args[i])) {
                printf("--ERRO: pthread_create()\n");
                exit(-1);
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
        clock_gettime(CLOCK_MONOTONIC, &fim);

        temp_exec = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
        media += temp_exec;
    }
    media /= 5;
   
    //printa resultado final
    printf("Número de primos até %lld: %d\n", N, count);
    printf("Tempo de execução médio(seg): %lf\n", media);

    f = fopen("resultados.txt", "a");
    if (f == NULL) {
        printf("ERROR: cannot open file \n");
        exit(4);
    }

    fprintf(f, "%lld\n", N);
    fprintf(f, "%d %.6f\n", nthreads, media);

    fclose(f);

    free(args);
    free(tid);

    return 0;
}