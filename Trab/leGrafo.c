#include<stdio.h>
#include<stdlib.h>
 
#define INF 1000000000

void imprime_matriz_adjacencia(int V, int *adjacencia) {
    int indice, peso;
    for(int i=0; i<V; i++) {
        for(int j=0; j<V; j++) {
            indice = i*V + j;
            peso = adjacencia[indice];
            if(peso == INF) { printf("INF "); }
            else { printf("%d ", peso); } 
        }
        printf("\n");
    }
}

int main(int argc, char*argv[]) {
    int *adjacencia; //matriz que será carregada do arquivo
    int V, E; //dimensoes da matriz
    FILE * arq; //descritor do arquivo de entrada
    size_t ret; //retorno da funcao de leitura no arquivo de entrada

    //recebe os argumentos de entrada
    if(argc < 2) {
        fprintf(stderr, "Digite: %s <arquivo entrada>\n", argv[0]);
        return 1;
    }

    //abre o arquivo para leitura binaria
    arq = fopen(argv[1], "rb");
    if(!arq) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }

    //le o número de vértices e arestas
    ret = fread(&V, sizeof(int), 1, arq);
    if(!ret) {
        fprintf(stderr, "Erro de leitura do número de vértices \n");
        return 3;
    }
    printf("Vertices: %d\n", V);
    ret = fread(&E, sizeof(int), 1, arq);
    if(!ret) {
        fprintf(stderr, "Erro de leitura dao número de arestas \n");
        return 3;
    }
    printf("Arestas: %d\n\n", E);
    //aloca memoria para a matriz
    adjacencia = (int*) malloc(sizeof(int) * V*V);
    if(!adjacencia) {
        fprintf(stderr, "Erro de alocao da memoria da matriz\n");
        return 3;
    }

    //carrega a matriz de elementos do tipo float do arquivo
    ret = fread(adjacencia, sizeof(int), V*V, arq);
    if(!ret) {
        fprintf(stderr, "Erro de leitura dos elementos da matriz de adjacência\n");
        return 4;
    }

    //imprime a matriz na saida padrao
    printf("Matriz de adjacencia:\n");
    imprime_matriz_adjacencia(V, adjacencia);

    //finaliza o uso das variaveis
    fclose(arq);
    free(adjacencia);
    return 0;
}
 