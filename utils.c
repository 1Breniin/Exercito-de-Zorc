#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "recrutamento.h"

Instancias* ler_entrada(const char* nome_arquivo) {
    FILE* f = fopen(nome_arquivo, "r");
    if (!f) {
        perror("Erro ao abrir o arquivo de entrada");
        exit(1);
    }

    Instancias* instancias = malloc(sizeof(Instancias));
    fscanf(f, "%d", &instancias->total_instancias);
    instancias->instancias = malloc(sizeof(Grafo*) * instancias->total_instancias);

    for (int k = 0; k < instancias->total_instancias; k++) {
        int P, D, W, C;
        fscanf(f, "%d %d %d %d", &P, &D, &W, &C);
        Grafo* grafo = criar_grafo(P);
        grafo->limite_distancia = D;
        grafo->limite_peso = W;
        grafo->quantidade_caminhos = C;

        for (int i = 0; i < P; i++) {
            int id, peso, habilidade;
            fscanf(f, "%d %d %d", &id, &peso, &habilidade);
            grafo->povos[id - 1].peso = peso;
            grafo->povos[id - 1].habilidade = habilidade;
        }

        for (int i = 0; i < C; i++) {
            int a, b, d;
            fscanf(f, "%d %d %d", &a, &b, &d);
            adicionar_aresta(grafo, a - 1, b - 1, d);
        }

        instancias->instancias[k] = grafo;
    }

    fclose(f);
    return instancias;
}

void escrever_saida(const char* nome_arquivo, Instancias* instancias) {
    FILE* out = fopen(nome_arquivo, "w");
    if (!out) {
        perror("Erro ao criar o arquivo de saída");
        exit(1);
    }

    for (int i = 0; i < instancias->total_instancias; i++) {
        Grafo* g = instancias->instancias[i];

        ResultadoRecrutamento* res_dyn = solucao_dinamica(g);
        ResultadoRecrutamento* res_heur = solucao_heuristica(g);

        fprintf(out, "[Dinamica] ");
        imprimir_resultado(res_dyn, out);

        fprintf(out, "[Heuristica] ");
        imprimir_resultado(res_heur, out);

        liberar_resultado(res_dyn);
        liberar_resultado(res_heur);
    }

    fclose(out);
}

void liberar_instancias(Instancias* instancias) {
    for (int i = 0; i < instancias->total_instancias; i++) {
        liberar_grafo(instancias->instancias[i]);
    }
    free(instancias->instancias);
    free(instancias);
}
