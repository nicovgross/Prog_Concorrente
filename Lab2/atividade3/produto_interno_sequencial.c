#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000

int main(int argc, char*argv[]) {
    float **vetores; //vetor que será gerada
    int n; //qtde de elementos do vetor
    float elem; //valor gerado para incluir no vetor
    int fator=1; //fator multiplicador para gerar números negativos
    double prod_interno = 0;
    FILE * arq; //descritor do arquivo de saida
    size_t ret; //retorno da funcao de escrita no arquivo de saida

    //recebe os argumentos de entrada
    if(argc < 2) {
        fprintf(stderr, "Digite: %s <dimensao> <arquivo binário>\n", argv[0]);
        return 1;
    }
    n = atoi(argv[1]);

    //aloca memoria para os vetores
    vetores = malloc(sizeof(float*) * 2);
    if(!vetores) {
        printf("Erro de alocao da memoria dos vetores\n");
        return 2;
    }
    for(int i=0; i<2; i++) {
        vetores[i] = malloc(sizeof(float) * n);
    }

    //preenche o vetor com valores float aleatorios
    srand(time(NULL));
    for(int i=0; i<2; i++) {
        for(int j=0; j<n; j++) {
            elem = (rand() % MAX)/3.0 * fator;
            vetores[i][j] = elem;
            fator*=-1;
        }
    }

    //imprimir na saida padrao o vetor gerado
    printf("Dimensão: %d\n", n);
    for(int i=0; i<=1; i++) {
        printf("Vetor %d: ", i+1);
        for(int j=0; j<n; j++) {
            printf("%f ",vetores[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    arq = fopen(argv[2], "wb");
    if(!arq) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }
    //escreve a dimensao
    ret = fwrite(&n, sizeof(int), 1, arq);
    //escreve os elementos dos vetores
    ret = fwrite(vetores[0], sizeof(float), n, arq);
    ret = fwrite(vetores[1], sizeof(float), n, arq);
    if(ret < n) {
        fprintf(stderr, "Erro de escrita no  arquivo\n");
        return 4;
    }

    for(int i=0; i<n; i++) {
        prod_interno += vetores[0][i] * vetores[1][i];
    }
    printf("Produto interno: %f\n", prod_interno);
    ret = fwrite(&prod_interno, sizeof(double), 1, arq);

    for (int i = 0; i<2; i++) {
        free(vetores[i]);
    } free(vetores);

    return 0;
} 
