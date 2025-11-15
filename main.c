/*
 * Grupo: Bombeiros Gulosos
 * Integrantes: Gabriel Franco da Rosa Messora - RA: 10438405
 * Integrantes: João Francisco - RA:10443666
 * Integrantes: Felipe Hideki Rodrigues Shinozaki - RA: 10438584
 * Integrantes: Jonas Fernando Nascimento de Melo - RA:10443398 
 * 
 * Link do vídeo do YouTube: https://www.youtube.com/watch?v=0SkJhX7E9nM
 * 
 * Referências:
 * - Algoritmo de Dijkstra baseado no enunciado do projeto.
 * - Implementação em C inspirada em livros de algoritmos como "Introduction to Algorithms" de Cormen et al.
 * - Uso de listas de adjacência para grafos.
 * - Não foi usado ChatGPT ou similares.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Definindo infinito como o maior valor possível para int
#define INF INT_MAX

// Estrutura para representar uma aresta (destino e peso)
typedef struct Edge {
    int dest;
    int weight;
    struct Edge* next;
} Edge;

// Estrutura para o grafo: array de listas de adjacência
typedef struct Graph {
    Edge** adjList;
    int numVertices;
} Graph;

// Função para criar um novo nó de aresta
Edge* createEdge(int dest, int weight) {
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->dest = dest;
    newEdge->weight = weight;
    newEdge->next = NULL;
    return newEdge;
}

// Função para criar o grafo
Graph* createGraph(int numVertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->adjList = (Edge**)malloc(numVertices * sizeof(Edge*));
    for (int i = 0; i < numVertices; i++) {
        graph->adjList[i] = NULL;
    }
    return graph;
}

// Função para adicionar uma aresta ao grafo
void addEdge(Graph* graph, int src, int dest, int weight) {
    // Ajuste para índices baseados em 0 (esquinas de 1 a n)
    src--;
    dest--;
    Edge* newEdge = createEdge(dest, weight);
    newEdge->next = graph->adjList[src];
    graph->adjList[src] = newEdge;
}

// Função para encontrar o vértice com menor distância não visitado
int minDistance(int dist[], int visited[], int numVertices) {
    int min = INF;
    int minIndex = -1;
    for (int v = 0; v < numVertices; v++) {
        if (!visited[v] && dist[v] < min) {
            min = dist[v];
            minIndex = v;
        }
    }
    return minIndex;
}

// Função para imprimir o caminho usando predecessores
void printPath(int pred[], int dest, FILE* outputFile) {
    if (pred[dest] == -1) {
        fprintf(outputFile, "%d ", dest + 1); // Ajuste para 1-based
        return;
    }
    printPath(pred, pred[dest], outputFile);
    fprintf(outputFile, "%d ", dest + 1);
}

// Função de Dijkstra modificada para calcular distâncias e predecessores
void dijkstra(Graph* graph, int src, int dest, int numVertices, int* totalTime, int pred[]) {
    int dist[numVertices];
    int visited[numVertices];

    // Inicializar distâncias e predecessores
    for (int i = 0; i < numVertices; i++) {
        dist[i] = INF;
        visited[i] = 0;
        pred[i] = -1;
    }
    dist[src] = 0;

    // Conjunto E: vértices não visitados
    for (int count = 0; count < numVertices - 1; count++) {
        int u = minDistance(dist, visited, numVertices);
        if (u == -1) break; // Nenhum vértice restante
        visited[u] = 1;

        // Atualizar vizinhos
        Edge* edge = graph->adjList[u];
        while (edge != NULL) {
            int v = edge->dest;
            if (!visited[v] && dist[u] != INF && dist[u] + edge->weight < dist[v]) {
                dist[v] = dist[u] + edge->weight;
                pred[v] = u;
            }
            edge = edge->next;
        }
    }

    // Tempo total para o destino
    *totalTime = dist[dest];
}

// Função principal
int main() {
    FILE* inputFile = fopen("entrada.txt", "r"); // Arquivo de entrada (ajuste o nome se necessário)
    if (inputFile == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    int fireCorner; // Esquina do incêndio
    int numCorners; // Número de esquinas

    fscanf(inputFile, "%d", &fireCorner);
    fscanf(inputFile, "%d", &numCorners);

    Graph* graph = createGraph(numCorners);

    // Ler triplas até 0
    while (1) {
        int src, dest, weight;
        fscanf(inputFile, "%d", &src);
        if (src == 0) break;
        fscanf(inputFile, "%d %d", &dest, &weight);
        addEdge(graph, src, dest, weight);
    }

    fclose(inputFile);

    // Ajuste para índices baseados em 0
    int src = 0; // Quartel sempre na esquina 1 (índice 0)
    int dest = fireCorner - 1;

    int pred[numCorners]; // Array de predecessores
    int totalTime;

    // Executar Dijkstra
    dijkstra(graph, src, dest, numCorners, &totalTime, pred);

    // Abrir arquivo de saída
    FILE* outputFile = fopen("saida.txt", "w");
    if (outputFile == NULL) {
        printf("Erro ao abrir o arquivo de saída.\n");
        return 1;
    }

    // Imprimir na tela e no arquivo
    printf("rota até a esquina #%d: ", fireCorner);
    fprintf(outputFile, "rota até a esquina #%d: ", fireCorner);
    printPath(pred, dest, stdout);
    printPath(pred, dest, outputFile);
    printf("\ntempo calculado para rota = %d min.\n", totalTime);
    fprintf(outputFile, "\ntempo calculado para rota = %d min.\n", totalTime);

    fclose(outputFile);

    // Liberar memória
    for (int i = 0; i < numCorners; i++) {
        Edge* edge = graph->adjList[i];
        while (edge != NULL) {
            Edge* temp = edge;
            edge = edge->next;
            free(temp);
        }
    }
    free(graph->adjList);
    free(graph);

    return 0;
}
