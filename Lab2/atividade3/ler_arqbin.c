#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *arq;
    long int n;
    float *vetor1, *vetor2;
    double prod_interno;
    size_t ret;

    if (argc < 2) {
        fprintf(stderr, "Digite: %s <arquivo_binario>\n", argv[0]);
        return 1;
    }

    arq = fopen(argv[1], "rb");
    if (!arq) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", argv[1]);
        return 2;
    }

    // Lê a dimensão
    ret = fread(&n, sizeof(long int), 1, arq);
    if (ret != 1) {
        fprintf(stderr, "Erro ao ler a dimensão\n");
        return 3;
    }

    vetor1 = malloc(sizeof(float) * n);
    vetor2 = malloc(sizeof(float) * n);
    if (!vetor1 || !vetor2) {
        fprintf(stderr, "Erro de alocação\n");
        return 4;
    }

    // Lê os dois vetores
    ret = fread(vetor1, sizeof(float), n, arq);
    if (ret != n) {
        fprintf(stderr, "Erro ao ler vetor 1\n");
        return 5;
    }

    ret = fread(vetor2, sizeof(float), n, arq);
    if (ret != n) {
        fprintf(stderr, "Erro ao ler vetor 2\n");
        return 6;
    }

    // Lê o produto interno
    ret = fread(&prod_interno, sizeof(double), 1, arq);
    if (ret != 1) {
        fprintf(stderr, "Erro ao ler produto interno\n");
        return 7;
    }

    fclose(arq);

    // Imprime os dados
    printf("Dimensão: %ld\n", n);
    printf("Vetor 1: ");
    for (long int i = 0; i < n; i++) {
        printf("%f ", vetor1[i]);
    } printf("\n");

    printf("Vetor 2: ");
    for (long int i = 0; i < n; i++) {
        printf("%f ", vetor2[i]);
    } printf("\n");

    printf("Produto Interno: %f\n", prod_interno);

    free(vetor1);
    free(vetor2);

    return 0;
}