#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

sem_t produtor, consumidor;
pthread_mutex_t mutex;
int M, N, C;
int *seq;
int prod_pos = 0, cons_pos = 0, count = 0;

int ehPrimo(long long int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void* Produtor(void* arg) {
    for (int i = 0; i < N; i++) {
        sem_wait(&produtor);

        pthread_mutex_lock(&mutex);
        prod_pos = i % M;
        seq[prod_pos] = i;
        pthread_mutex_unlock(&mutex);

        sem_post(&consumidor);
    }

    // Envia sinais extras para liberar consumidores que possam estar bloqueados
    for (int i = 0; i < C; i++) {
        sem_post(&consumidor);
    }

    pthread_exit(NULL);
}

void* Consumidor(void* arg) {
    int id = *((int*) arg);
    int *local_count = malloc(sizeof(int));
    *local_count = 0;

    while (1) {
        sem_wait(&consumidor);

        pthread_mutex_lock(&mutex);
        if (cons_pos >= N) {
            pthread_mutex_unlock(&mutex);
            sem_post(&consumidor); // avisa outros consumidores que podem estar esperando
            break;
        }

        int pos = cons_pos % M;
        int num = seq[pos];
        cons_pos++;
        pthread_mutex_unlock(&mutex);

        if (ehPrimo(num)) (*local_count)++;

        sem_post(&produtor);
    }

    pthread_mutex_lock(&mutex);
    count += *local_count;
    pthread_mutex_unlock(&mutex);

    pthread_exit((void*) local_count);
}

int main(int argc, char* argv[]) {
    pthread_t *tid;

    pthread_mutex_init(&mutex, NULL);

    if (argc < 4) {
        printf("Uso: %s <tamanho da sequência> <tamanho do canal> <número de consumidores>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    C = atoi(argv[3]);

    seq = (int*) malloc(sizeof(int) * M);
    tid = (pthread_t*) malloc(sizeof(pthread_t) * (C + 1));
    int *ids = (int*) malloc(sizeof(int) * C);

    sem_init(&produtor, 0, M); // espaço vazio no buffer
    sem_init(&consumidor, 0, 0); // buffer começa vazio

    pthread_create(&tid[0], NULL, Produtor, NULL); //cria a thread produtora

    //cria as threads consumidoras
    for (int i = 0; i < C; i++) {
        ids[i] = i + 1;
        pthread_create(&tid[i + 1], NULL, Consumidor, &ids[i]);
    }

    void *ret;
    for (int i = 0; i < C + 1; i++) {
        pthread_join(tid[i], &ret);
        if (i != 0 && ret != NULL) {
            int *valor = (int*) ret;
            printf("Consumidor %d encontrou %d primos\n", i, *valor);
            free(valor);
        }
    }

    printf("Total de primos: %d\n", count);

    // Libera a memória alocada
    free(seq);
    free(tid);
    free(ids);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&produtor);
    sem_destroy(&consumidor);

    return 0;
}
