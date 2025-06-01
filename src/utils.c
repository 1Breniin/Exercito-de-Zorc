#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "recrutamento.h"

// Função para ler as instâncias do arquivo de entrada
Instancias* ler_entrada(const char* nome_arquivo) {
    FILE* f = fopen(nome_arquivo, "r");
    if (!f) {
        //Se nao conseguir abrir o arquivo, ele imprime uma mensagem de erro
        perror("Erro ao abrir o arquivo de entrada");
        exit(1);
    }

    // Aloca a estrutura principal que vai guardar todas as instâncias
    Instancias* instancias = malloc(sizeof(Instancias));
    if (fscanf(f, "%d", &instancias->total_instancias) != 1) {
        fprintf(stderr, "Erro ao ler o número de instâncias\n");
        fclose(f);
        free(instancias);
        exit(1);
    }

    // Verifica se o número de instâncias faz sentido
    if (instancias->total_instancias <= 0) {
        fprintf(stderr, "Número de instâncias inválido: %d\n", instancias->total_instancias);
        fclose(f);
        free(instancias);
        exit(1);
    }

    // Aloca o vetor de ponteiros para grafos (uma por instância)
    instancias->instancias = malloc(sizeof(Grafo*) * instancias->total_instancias);

    // Lê os dados de cada instância
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

        // Cria um grafo com P povos
        Grafo* grafo = criar_grafo(P);
        grafo->limite_distancia = D;
        grafo->limite_peso = W;
        grafo->quantidade_caminhos = C;

        // Lê os dados de cada povo (peso e habilidade)
        for (int i = 0; i < P; i++) {
            int id, peso, habilidade;
            if (fscanf(f, "%d %d %d", &id, &peso, &habilidade) != 3) {
                fprintf(stderr, "Erro ao ler os dados do povo %d da instância %d\n", i + 1, k + 1);
                fclose(f);
                liberar_instancias(instancias);
                exit(1);
            }

            // Verifica se os dados do povo são válidos
            if (id < 1 || id > P || peso < 0 || habilidade < 0) {
                fprintf(stderr, "Dados inválidos do povo %d na instância %d: id=%d peso=%d habilidade=%d\n",
                        i + 1, k + 1, id, peso, habilidade);
                fclose(f);
                liberar_instancias(instancias);
                exit(1);
            }

            // Armazena os dados no grafo (convertendo id para índice)
            grafo->povos[id - 1].peso = peso;
            grafo->povos[id - 1].habilidade = habilidade;
        }

        // Lê os caminhos entre os povos (arestas do grafo)
        for (int i = 0; i < C; i++) {
            int a, b, d;
            if (fscanf(f, "%d %d %d", &a, &b, &d) != 3) {
                fprintf(stderr, "Erro ao ler o caminho %d da instância %d\n", i + 1, k + 1);
                fclose(f);
                liberar_instancias(instancias);
                exit(1);
            }

            // Valida os dados do caminho
            if (a < 1 || a > P || b < 1 || b > P || d <= 0) {
                fprintf(stderr, "Dados inválidos do caminho %d na instância %d: a=%d b=%d d=%d\n",
                        i + 1, k + 1, a, b, d);
                fclose(f);
                liberar_instancias(instancias);
                exit(1);
            }

            // Adiciona a aresta no grafo (convertendo para índice base 0)
            adicionar_aresta(grafo, a - 1, b - 1, d);
        }

        // Armazena o grafo na estrutura principal
        instancias->instancias[k] = grafo;
    }

    fclose(f);
    return instancias;
}

//Função para escrever os resultados no arquivo de saida
void escrever_saida(const char* nome_arquivo, Instancias* instancias) {
    FILE* out = fopen(nome_arquivo, "w");
    if (!out) {
        perror("Erro ao criar o arquivo de saída");
        exit(1);
    }

    for (int i = 0; i < instancias->total_instancias; i++) {
        Grafo* g = instancias->instancias[i];

        ResultadoRecrutamento* res = solucao_dinamica(g);
        if (res != NULL) {
            imprimir_resultado(res, out);
            liberar_resultado(res);
        } else {
            fprintf(out, "0\n");
        }
    }

    fclose(out);
}

//Libera toda a memoria usada pelas as instancias
void liberar_instancias(Instancias* instancias) {
    for (int i = 0; i < instancias->total_instancias; i++) {
        liberar_grafo(instancias->instancias[i]);
    }
    free(instancias->instancias);
    free(instancias);
}