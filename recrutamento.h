#ifndef RECRUTAMENTO_H
#define RECRUTAMENTO_H

#include "grafo.h"

typedef struct {
    int* caminho;
    int* soldados_por_povo;
    int tamanho_caminho;
    int habilidade_total;
} ResultadoRecrutamento;


ResultadoRecrutamento* solucao_dinamica(Grafo* grafo);
ResultadoRecrutamento* solucao_heuristica(Grafo* grafo);

void liberar_resultado(ResultadoRecrutamento* resultado);
void imprimir_resultado(ResultadoRecrutamento* resultado, FILE* out);

#endif