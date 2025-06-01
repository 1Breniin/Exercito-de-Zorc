#ifndef UTILS_H
#define UTILS_H

#include "grafo.h"

typedef struct {
    int total_instancias;
    Grafo** instancias;
} Instancias;

Instancias* ler_entrada(const char* nome_arquivo);

void escrever_saida(const char* nome_arquivo, Instancias* instancias);

void liberar_instancias(Instancias* instancias);

#endif