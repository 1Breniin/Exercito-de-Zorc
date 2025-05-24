#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

Grafo* criar_grafo(int quantidade_povos) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->quantidade_povos = quantidade_povos;
    for (int i = 0; i < quantidade_povos; i++) {
        grafo->adj[i] = NULL;
        grafo->qtd_adj[i] = 0;
    }
    return grafo;
}

void adicionar_aresta(Grafo* grafo, int origem, int destino, int distancia) {
    grafo->adj[origem] = (Aresta*)realloc(grafo->adj[origem], sizeof(Aresta) * (grafo->qtd_adj[origem] + 1));
    grafo->adj[origem][grafo->qtd_adj[origem]].destino = destino;
    grafo->adj[origem][grafo->qtd_adj[origem]].distancia = distancia;
    grafo->qtd_adj[origem]++;

    // Arestas são bidirecionais
    grafo->adj[destino] = (Aresta*)realloc(grafo->adj[destino], sizeof(Aresta) * (grafo->qtd_adj[destino] + 1));
    grafo->adj[destino][grafo->qtd_adj[destino]].destino = origem;
    grafo->adj[destino][grafo->qtd_adj[destino]].distancia = distancia;
    grafo->qtd_adj[destino]++;
}

void liberar_grafo(Grafo* grafo) {
    for (int i = 0; i < grafo->quantidade_povos; i++) {
        free(grafo->adj[i]);
    }
    free(grafo);
}