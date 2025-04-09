#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#define MAX 1000

typedef struct {
    int num_elem;
    int num_threads;
    int tid;
    float *vetor1;
    float *vetor2;
} t_Args; 

void* ProdutoInterno(void *arg) {
    t_Args *args = (t_Args*) arg;
    int inicio, fim, bloco;
    double soma = 0;

    bloco = floor(args->num_elem / args->num_threads);
    inicio = args->tid * bloco; 
    fim = inicio + bloco;

    if(args->tid == args->num_threads-1) { fim = args->num_elem; }

    for (int i = inicio; i < fim; i++) {
        soma += args->vetor1[i] * args->vetor2[i];
    }

    double *ret = malloc(sizeof(double));
    *ret = soma;
    pthread_exit((void*) ret);
}

int main(int argc, char*argv[]) {
    int num_threads;
    pthread_t *tid;
    t_Args *args;
    float *vetor1, *vetor2;
    int n; //qtde de elementos do vetor
    double prod_interno_conc = 0;
    FILE * arq; //descritor do arquivo de saida
    size_t ret; //retorno da funcao de escrita no arquivo de saida

    if(argc < 3) {
        fprintf(stderr, "Digite: %s <num_threads> <arquivo_binario>\n", argv[0]);
        return 1;
    }

    num_threads = atoi(argv[1]);

    arq = fopen(argv[2], "rb");
    if (!arq) {
        fprintf(stderr, "Erro ao abrir arquivo\n");
        return 2;
    }

    fread(&n, sizeof(int), 1, arq);
    if(num_threads > n) { 
        fprintf(stderr, "Número de threads excedeu a dimensão dos vetores");
        return 1;
    }

    vetor1 = malloc(sizeof(float) * n);
    vetor2 = malloc(sizeof(float) * n);
    fread(vetor1, sizeof(float), n, arq);
    fread(vetor2, sizeof(float), n, arq);

    tid = malloc(sizeof(pthread_t) * num_threads);
    args = malloc(sizeof(t_Args) * num_threads);

    for (int i = 0; i < num_threads; i++) {
        args[i].tid = i;
        args[i].num_elem = n;
        args[i].num_threads = num_threads;
        args[i].vetor1 = vetor1;
        args[i].vetor2 = vetor2;
        printf("Criando thread %d\n", i);
        ret = pthread_create(&tid[i], NULL, ProdutoInterno, (void*) &args[i]);
        if(ret) { printf("Erro ao criar thread"); }
    }

    for (int i = 0; i < num_threads; i++) {
        double *soma;
        pthread_join(tid[i], (void**) &soma);
        printf("Thread %d retornou %f\n", i, *soma);
        prod_interno_conc += *soma;
        free(soma);
    }

    printf("Produto Interno calculado concorrentemente: %f\n", prod_interno_conc);

    double *prod_interno_seq = malloc(sizeof(double));
    fread(prod_interno_seq, sizeof(double), 1, arq);
    printf("Produto interno calculado sequencialmente: %f\n", *prod_interno_seq);

    double var = (*prod_interno_seq - prod_interno_conc) / *prod_interno_seq;
    printf("variação relativa: %f", var);

    fclose(arq);
    free(vetor1);
    free(vetor2);
    free(tid);
    free(args);

    return 0;
}