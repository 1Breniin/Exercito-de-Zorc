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
    if (fscanf(f, "%d", &instancias->total_instancias) != 1) {
        fprintf(stderr, "Erro ao ler o número de instâncias\n");
        fclose(f);
        free(instancias);
        exit(1);
    }

    if (instancias->total_instancias <= 0) {
        fprintf(stderr, "Número de instâncias inválido: %d\n", instancias->total_instancias);
        fclose(f);
        free(instancias);
        exit(1);
    }

    instancias->instancias = malloc(sizeof(Grafo*) * instancias->total_instancias);

    for (int k = 0; k < instancias->total_instancias; k++) {
        int P, D, W, C;
        if (fscanf(f, "%d %d %d %d", &P, &D, &W, &C) != 4) {
            fprintf(stderr, "Erro ao ler os parâmetros da instância %d\n", k + 1);
            fclose(f);
            liberar_instancias(instancias);
            exit(1);
        }

        if (P <= 0 || D <= 0 || W <= 0 || C < 0) {
            fprintf(stderr, "Parâmetros inválidos na instância %d: P=%d D=%d W=%d C=%d\n", 
                    k + 1, P, D, W, C);
            fclose(f);
            liberar_instancias(instancias);
            exit(1);
        }

        Grafo* grafo = criar_grafo(P);
        grafo->limite_distancia = D;
        grafo->limite_peso = W;
        grafo->quantidade_caminhos = C;

        for (int i = 0; i < P; i++) {
            int id, peso, habilidade;
            if (fscanf(f, "%d %d %d", &id, &peso, &habilidade) != 3) {
                fprintf(stderr, "Erro ao ler os dados do povo %d da instância %d\n", i + 1, k + 1);
                fclose(f);
                liberar_instancias(instancias);
                exit(1);
            }

            if (id < 1 || id > P || peso < 0 || habilidade < 0) {
                fprintf(stderr, "Dados inválidos do povo %d na instância %d: id=%d peso=%d habilidade=%d\n",
                        i + 1, k + 1, id, peso, habilidade);
                fclose(f);
                liberar_instancias(instancias);
                exit(1);
            }

            grafo->povos[id - 1].peso = peso;
            grafo->povos[id - 1].habilidade = habilidade;
        }

        for (int i = 0; i < C; i++) {
            int a, b, d;
            if (fscanf(f, "%d %d %d", &a, &b, &d) != 3) {
                fprintf(stderr, "Erro ao ler o caminho %d da instância %d\n", i + 1, k + 1);
                fclose(f);
                liberar_instancias(instancias);
                exit(1);
            }

            if (a < 1 || a > P || b < 1 || b > P || d <= 0) {
                fprintf(stderr, "Dados inválidos do caminho %d na instância %d: a=%d b=%d d=%d\n",
                        i + 1, k + 1, a, b, d);
                fclose(f);
                liberar_instancias(instancias);
                exit(1);
            }

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
