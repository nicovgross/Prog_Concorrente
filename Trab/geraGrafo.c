#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int pesoMax = 15;
int pesoMin = -15;
#define INF 1000000000

void geraGrafo(int V, int E, int *adjacencia) {
    int total = V * V;

    // Inicializa todas as posições com INF
    for (int i = 0; i < total; i++) {
        adjacencia[i] = INF;
    }

    // Define distância 0 para as autoarestas
    for (int i = 0; i < V; i++) {
        adjacencia[i * V + i] = 0;
    }

    // Gera E arestas aleatórias
    int arestasCriadas = 0;
    while (arestasCriadas < E) {
        //escolhe 2 vértices aleatóreos
        int u = rand() % V;
        int v = rand() % V;

        // evita autoarestas duplicadas e sobreposição de arestas
        if (u != v && adjacencia[u * V + v] == INF) {
            int peso = (rand() % (pesoMax - pesoMin + 1)) + pesoMin;
            adjacencia[u * V + v] = peso;
            arestasCriadas++;
        }
    }
}

int main(int argc, char* argv[]) {
    int *adjacencia; //matriz de adjacência do grafo
    int V, E; // número de vértices e arestas respectivamente
    FILE *arq; //arquivo de saída
    size_t ret; //retorno da funcao de leitura no arquivo de saída

    if(argc < 4) {
        fprintf(stderr, "Digite: %s <vertices> <arestas> <arquivo saida>\n", argv[0]);
        return 1;
    }
    V = atoi(argv[1]); //lê o número de vértices
    E = atoi(argv[2]); //lê o número de arestas
    adjacencia = (int*) malloc(sizeof(int) * V*V); //aloca espaço para a matriz de adjacência
    if(!adjacencia) {
        fprintf(stderr, "Erro de alocao da memoria da matriz\n");
        return 2;
    }

    geraGrafo(V, E, adjacencia); //gera um grafo com V vértices e E arestas

    arq = fopen(argv[3], "wb");
    if(!arq) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }

    ret = fwrite(&V, sizeof(int), 1, arq);
    ret = fwrite(&E, sizeof(int), 1, arq);
    //escreve os elementos da matriz
    ret = fwrite(adjacencia, sizeof(int), V*V, arq);
    if(ret < V) {
        fprintf(stderr, "Erro de escrita no  arquivo\n");
        return 4;
    }

    fclose(arq);
    free(adjacencia);

    return 0;
}