#ifndef RECRUTAMENTO_H
#define RECRUTAMENTO_H

#include "grafo.h"


typedef struct {
    int* caminho;              // Array com a sequência de povos visitados
    int* soldados_por_povo;    // Quantidade de soldados recrutados em cada povo
    int tamanho_caminho;       // Tamanho do caminho percorrido
    int habilidade_total;      // Soma total das habilidades dos soldados recrutados
} ResultadoRecrutamento;


ResultadoRecrutamento* solucao_heuristica(Grafo* grafo);
ResultadoRecrutamento* solucao_dinamica(Grafo* grafo);
void imprimir_resultado(ResultadoRecrutamento* resultado, FILE* out);
void liberar_resultado(ResultadoRecrutamento* resultado);

#endif