#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recrutamento.h"
#include "grafo.h"

// Limites do problema
#define MAX_W 1000000  // Limite máximo de peso que pode ser carregado
#define MAX_D 10000    // Limite máximo de distância que pode ser percorrida
#define MAX_CAMINHO 100 // Tamanho máximo do caminho a ser percorrido

/**
 * @brief Estrutura para armazenar o estado da programação dinâmica
 * 
 * Esta estrutura é usada internamente pela solução dinâmica para armazenar
 * o estado atual do recrutamento, incluindo o caminho percorrido, peso usado,
 * distância percorrida e quantidade de soldados recrutados.
 */
typedef struct {
    int habilidade;           // Habilidade total dos soldados recrutados
    int peso_usado;           // Peso total carregado até o momento
    int distancia_usada;      // Distância total percorrida
    int* caminho;             // Sequência de povos visitados
    int tamanho_caminho;      // Tamanho do caminho percorrido
    int* soldados_por_povo;   // Quantidade de soldados recrutados por povo
} EstadoDP;

/**
 * @brief Função auxiliar para calcular o melhor resultado possível a partir de um estado
 * 
 * Esta função implementa a recursão da programação dinâmica, calculando o melhor
 * caminho possível a partir de um estado atual, considerando os limites de peso
 * e distância. Utiliza memoização para evitar recálculos desnecessários.
 * 
 * @param grafo Grafo que representa o mapa
 * @param atual Povo atual sendo visitado
 * @param peso_usado Peso já carregado
 * @param distancia_usada Distância já percorrida
 * @param visitado Array indicando quais povos já foram visitados
 * @param memo Array para memoização dos resultados
 * @return EstadoDP* Melhor estado possível a partir do estado atual
 */
EstadoDP* calcular_melhor_estado(Grafo* grafo, int atual, int peso_usado, int distancia_usada, 
                                int* visitado, EstadoDP** memo) {
    if (memo[atual] != NULL) {
        return memo[atual];
    }

    EstadoDP* estado = (EstadoDP*)malloc(sizeof(EstadoDP));
    estado->caminho = (int*)malloc(sizeof(int) * MAX_CAMINHO);
    estado->soldados_por_povo = (int*)calloc(grafo->quantidade_povos, sizeof(int));
    estado->tamanho_caminho = 0;
    estado->habilidade = 0;
    estado->peso_usado = peso_usado;
    estado->distancia_usada = distancia_usada;

    visitado[atual] = 1;
    estado->caminho[estado->tamanho_caminho++] = atual;

    if (peso_usado < grafo->limite_peso) {
        int capacidade = grafo->limite_peso - peso_usado;
        int qtd_soldados = grafo->povos[atual].peso > 0 ? capacidade / grafo->povos[atual].peso : 0;
        estado->soldados_por_povo[atual] = qtd_soldados;
        estado->habilidade = qtd_soldados * grafo->povos[atual].habilidade;
        estado->peso_usado += qtd_soldados * grafo->povos[atual].peso;
    }

    int melhor_habilidade = 0;
    EstadoDP* melhor_estado = NULL;

    for (int i = 0; i < grafo->qtd_adj[atual]; i++) {
        int viz = grafo->adj[atual][i].destino;
        int dist = grafo->adj[atual][i].distancia;

        if (!visitado[viz] && distancia_usada + dist <= grafo->limite_distancia) {
            EstadoDP* estado_viz = calcular_melhor_estado(grafo, viz, 
                                                        estado->peso_usado,
                                                        estado->distancia_usada + dist,
                                                        visitado, memo);

            if (estado_viz != NULL && estado_viz->habilidade > melhor_habilidade) {
                melhor_habilidade = estado_viz->habilidade;
                melhor_estado = estado_viz;
            }
        }
    }

    if (melhor_estado != NULL) {
        estado->habilidade += melhor_estado->habilidade;
        estado->distancia_usada = melhor_estado->distancia_usada;
        
        for (int i = 0; i < melhor_estado->tamanho_caminho; i++) {
            estado->caminho[estado->tamanho_caminho++] = melhor_estado->caminho[i];
            estado->soldados_por_povo[melhor_estado->caminho[i]] = 
                melhor_estado->soldados_por_povo[melhor_estado->caminho[i]];
        }
    }

    memo[atual] = estado;
    visitado[atual] = 0;

    return estado;
}

/**
 * @brief Implementa uma solução ótima usando programação dinâmica
 * 
 * Esta função implementa uma solução exata usando programação dinâmica com
 * memoização. A estratégia é calcular o melhor caminho possível a partir de
 * cada estado, considerando o peso usado, distância percorrida e povos visitados.
 * 
 * @param grafo Grafo que representa o mapa com os povos e suas conexões
 * @return ResultadoRecrutamento* Resultado do recrutamento com o caminho e soldados
 */
ResultadoRecrutamento* solucao_dinamica(Grafo* grafo) {
    EstadoDP** memo = (EstadoDP**)calloc(grafo->quantidade_povos, sizeof(EstadoDP*));
    int* visitado = (int*)calloc(grafo->quantidade_povos, sizeof(int));

    EstadoDP* melhor_estado = calcular_melhor_estado(grafo, 0, 0, 0, visitado, memo);

    ResultadoRecrutamento* res = (ResultadoRecrutamento*)malloc(sizeof(ResultadoRecrutamento));
    res->caminho = melhor_estado->caminho;
    res->soldados_por_povo = melhor_estado->soldados_por_povo;
    res->tamanho_caminho = melhor_estado->tamanho_caminho;
    res->habilidade_total = melhor_estado->habilidade;

    for (int i = 0; i < grafo->quantidade_povos; i++) {
        if (memo[i] != NULL && memo[i] != melhor_estado) {
            free(memo[i]->caminho);
            free(memo[i]->soldados_por_povo);
            free(memo[i]);
        }
    }
    free(memo);
    free(visitado);
    free(melhor_estado);

    return res;
}

/**
 * @brief Implementa uma solução heurística (gulosa) para o problema de recrutamento
 * 
 * Esta função implementa uma solução aproximada usando uma abordagem gulosa.
 * A estratégia é sempre escolher o próximo povo que oferece a maior habilidade
 * por soldado, respeitando os limites de peso e distância.
 * 
 * @param grafo Grafo que representa o mapa com os povos e suas conexões
 * @return ResultadoRecrutamento* Resultado do recrutamento com o caminho e soldados
 */
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
            qtd_soldados = grafo->povos[atual].peso > 0 ? capacidade / grafo->povos[atual].peso : 0;
            max_habilidade = qtd_soldados * grafo->povos[atual].habilidade;

            peso_usado += qtd_soldados * grafo->povos[atual].peso;
            res->soldados_por_povo[atual] = qtd_soldados;
            res->habilidade_total += max_habilidade;
        }

        res->caminho[res->tamanho_caminho++] = atual;

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

/**
 * @brief Imprime o resultado do recrutamento em um arquivo
 * 
 * @param resultado Resultado do recrutamento a ser impresso
 * @param out Arquivo onde o resultado será impresso
 */
void imprimir_resultado(ResultadoRecrutamento* resultado, FILE* out) {
    fprintf(out, "%d", resultado->habilidade_total);
    for (int i = 0; i < resultado->tamanho_caminho; i++) {
        int p = resultado->caminho[i];
        fprintf(out, " %d %d", p + 1, resultado->soldados_por_povo[p]);
    }
    fprintf(out, "\n");
}

/**
 * @brief Libera a memória alocada para o resultado do recrutamento
 * 
 * @param resultado Resultado do recrutamento a ser liberado
 */
void liberar_resultado(ResultadoRecrutamento* resultado) {
    free(resultado->caminho);
    free(resultado->soldados_por_povo);
    free(resultado);
}
