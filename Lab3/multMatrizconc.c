/* Multiplicacao de matriz-matriz (considerando matrizes quadradas) */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

float *mat1; //matriz 1
float *mat2; //matriz 2
float *saida; //matriz de saida

typedef struct{
    int id; //identificador do elemento que a thread ira processar
    int nthreads; //numero de threads
    int linhasMat1;
    int colunasMat1;
    int linhasMat2;
    int colunasMat2;
} tArgs;

//funcao das threads
void* multiplicaMatriz(void *arg) {
    tArgs *args = (tArgs*) arg;
    for(int i = args->id; i < args->linhasMat1; i += args->nthreads){
        for(int j = 0; j < args->colunasMat2; j++) {
            for(int k = 0; k < args->linhasMat2; k++) {
                saida[i*(args->linhasMat1) + j] += mat1[i*(args->linhasMat1) + k] * mat2[k*(args->linhasMat2) + j];
            }
        }
    }
    pthread_exit(NULL);
}

//carrega as matrizes de entrada, retorna a qtde de linhas e colunas e a matriz/vetor preenchido
int carregaEntrada(FILE *descritorArquivo, float **entrada, int *linhas, int *colunas) {
    size_t ret; //retorno da funcao de leitura no arquivo de entrada
    int tam; //tamanho da matriz

    //le as dimensoes 
    ret = fread(linhas, sizeof(int), 1, descritorArquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes no arquivo \n");
        return 1;
    }
    ret = fread(colunas, sizeof(int), 1, descritorArquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes no arquivo \n");
        return 2;
    }
    tam = (*linhas) * (*colunas); //calcula a qtde de elementos

    //aloca memoria
    *entrada = (float*) malloc(sizeof(float) * tam);
    if(!entrada) {
        fprintf(stderr, "Erro de alocao da memoria\n");
        return 3;
    }

    //carrega a entrada
    ret = fread(*entrada, sizeof(float), tam, descritorArquivo);
    if(ret < tam) {
        fprintf(stderr, "Erro de leitura dos elementos da estrutura de entrada\n");
        return 4;
    }
    return 0;
}

void printMatriz(float *matriz, int linhas, int colunas) {
    for(int i=0; i<linhas; i++) {
        for(int j=0; j<colunas; j++) {
            printf("%f ", matriz[i*linhas + j]);
        }
        printf("\n");
    }
    printf("\n");
}

//fluxo principal
int main(int argc, char* argv[]) {
    int nthreads; //numero de threads
    pthread_t *tid; //identificadores das threads no sistema
    tArgs *args; //identificadores locais das threads e dimensao
    double inicio, fim, delta, total=0;
    FILE *descritorArquivoMatriz, *descritorArquivoVetor; //descritores do arquivo de entrada
    int linhasMat1, colunasMat1, linhasMat2, colunasMat2;


    GET_TIME(inicio);
    //leitura e avaliacao dos parametros de entrada
    if(argc<4) {
        printf("Digite: %s <arquivo matriz> <arquivo vetor> <numero de threads>\n", argv[0]);
        return 1;
    }

    //abre o arquivo com a matriz para leitura binaria
    descritorArquivoMatriz = fopen(argv[1], "rb");
    if(!descritorArquivoMatriz) {
        fprintf(stderr, "Erro de abertura do arquivo com a matriz\n");
        return 2;
    }
    //carrega a matriz
    if(carregaEntrada(descritorArquivoMatriz, &mat1, &linhasMat1, &colunasMat1)) { return 3; }

    //abre o arquivo com o vetor para leitura binaria
    descritorArquivoVetor = fopen(argv[2], "rb");
    if(!descritorArquivoVetor) {
        fprintf(stderr, "Erro de abertura do arquivo com o vetor\n");
        return 2;
    }
    //carrega o vetor
    if(carregaEntrada(descritorArquivoVetor, &mat2, &linhasMat2, &colunasMat2)) { return 3; }

    //verifica se as dimensoes do vetor e matriz sao compativeis (assume matriz quadrada)	   
    if(colunasMat1 != linhasMat2) {
        fprintf(stderr, "Erro dimensão das entradas\n");
    }
    //limita o numero de threads ao numero de linhas da matriz
    nthreads = atoi(argv[3]);
    if (nthreads > linhasMat1) nthreads=linhasMat1;

    //aloca espaço para o vetor de saida
    saida = (float *) malloc(sizeof(float) * linhasMat1*colunasMat2);
    if (saida == NULL) {printf("ERRO--malloc\n"); return 2;}

    GET_TIME(fim);
    delta = fim - inicio;
    //printf("Dimensao, Nthreads, Inicialização, Multiplicação, Finalização, Total\n");
    printf("%d, %d, ", linhasMat1, nthreads);
    printf("%lf, ", delta);
    total = delta;

    //multiplicacao da matriz pelo vetor
    GET_TIME(inicio);
    //alocacao das estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("ERRO--malloc"); return 4;}
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL) {puts("ERRO--malloc"); return 4;}
    //criacao das threads
    for(int i=0; i<nthreads; i++) {
        (args+i)->id = i;
        (args+i)->nthreads = nthreads;
        (args+i)->linhasMat1 = linhasMat1;
        (args+i)->colunasMat1 = colunasMat1;
        (args+i)->linhasMat2 = linhasMat2;
        (args+i)->colunasMat2 = colunasMat2;
        if(pthread_create(tid+i, NULL, multiplicaMatriz, (void*) (args+i))){
            puts("ERRO--pthread_create"); return 3;
        }
    } 
    //espera pelo termino da threads
    for(int i=0; i<nthreads; i++) {
        pthread_join(*(tid+i), NULL);
    }
    GET_TIME(fim);   
    delta = fim - inicio;
    printf("%lf, \n", delta);
    total += delta;

    printMatriz(mat1, args->linhasMat1, args->colunasMat1); printf("\n");
    printMatriz(mat2, args->linhasMat2, args->colunasMat2); printf("\n");
    printMatriz(saida, args->linhasMat1, args->colunasMat2);

    //-----------------------------------------
    //liberacao da memoria
    GET_TIME(inicio);
    free(mat1);
    free(mat2);
    free(saida);
    free(args);
    free(tid);
    GET_TIME(fim)   
    delta = fim - inicio;
    printf("%lf, ", delta);
    total += delta;
    printf("%lf\n", total);

    return 0;
}
