#ifndef GRAFO_H
#define GRAFO_H

#define MAX_POVOS 100

typedef struct {
    int peso;       // Peso dos soldados
    int habilidade; // Habilidade dos soldados
} Povo;

typedef struct {
    int destino;
    int distancia;
} Aresta;

typedef struct {
    int quantidade_povos;
    int limite_distancia;
    int limite_peso;
    int quantidade_caminhos;

    Povo povos[MAX_POVOS];
    Aresta* adj[MAX_POVOS];
    int qtd_adj[MAX_POVOS];
} Grafo;

Grafo* criar_grafo(int quantidade_povos);
void adicionar_aresta(Grafo* grafo, int origem, int destino, int distancia);
void liberar_grafo(Grafo* grafo);

#endif