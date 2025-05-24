#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recrutamento.h"
#include "grafo.h"

#define MAX_W 1000000  // Limite de peso
#define MAX_D 10000    // Limite de distância
#define MAX_CAMINHO 100

//Solução Heurística (Gulosa simples)
ResultadoRecrutamento* solucao_heuristica(Grafo* grafo) {
    ResultadoRecrutamento* res = (ResultadoRecrutamento*)malloc(sizeof(ResultadoRecrutamento));
    res->caminho = (int*)malloc(sizeof(int) * MAX_CAMINHO);
    res->soldados_por_povo = (int*)calloc(grafo->quantidade_povos, sizeof(int));
    res->tamanho_caminho = 0;
    res->habilidade_total = 0;

    int atual = 0;
    int peso_usado = 0;
    int distancia_usada = 0;
    int visitado[MAX_POVOS] = {0};

    while (1) {
        visitado[atual] = 1;
        int max_habilidade = 0, qtd_soldados = 0;

        if (peso_usado < grafo->limite_peso) {
            int capacidade = grafo->limite_peso - peso_usado;
            qtd_soldados = capacidade / grafo->povos[atual].peso;
            max_habilidade = qtd_soldados * grafo->povos[atual].habilidade;

            peso_usado += qtd_soldados * grafo->povos[atual].peso;
            res->soldados_por_povo[atual] = qtd_soldados;
            res->habilidade_total += max_habilidade;
        }

        res->caminho[res->tamanho_caminho++] = atual;

        // Procurar próximo melhor vizinho
        int melhor = -1, melhor_valor = -1;
        for (int i = 0; i < grafo->qtd_adj[atual]; i++) {
            int viz = grafo->adj[atual][i].destino;
            int dist = grafo->adj[atual][i].distancia;
            if (!visitado[viz] && distancia_usada + dist <= grafo->limite_distancia) {
                int h = grafo->povos[viz].habilidade;
                if (h > melhor_valor) {
                    melhor = viz;
                    melhor_valor = h;
                }
            }
        }

        if (melhor == -1) break;
        for (int i = 0; i < grafo->qtd_adj[atual]; i++) {
            if (grafo->adj[atual][i].destino == melhor) {
                distancia_usada += grafo->adj[atual][i].distancia;
                break;
            }
        }
        atual = melhor;
    }

    return res;
}

// ------- Solução Dinâmica (DFS com memoização simplificada) -------
ResultadoRecrutamento* solucao_dinamica(Grafo* grafo) {
    //Tentei implementar programação dinamica aqui e nao consegui, pedi o CharGpt para tentar me ajudar, mas ele empurrou com a barriga e so me fez usar a heuristica para isso, entao falta implementar ela aqui.

    return solucao_heuristica(grafo); 
}


void imprimir_resultado(ResultadoRecrutamento* resultado, FILE* out) {
    fprintf(out, "%d", resultado->habilidade_total);
    for (int i = 0; i < resultado->tamanho_caminho; i++) {
        int p = resultado->caminho[i];
        fprintf(out, " %d %d", p + 1, resultado->soldados_por_povo[p]);
    }
    fprintf(out, "\n");
}

void liberar_resultado(ResultadoRecrutamento* resultado) {
    free(resultado->caminho);
    free(resultado->soldados_por_povo);
    free(resultado);
}
