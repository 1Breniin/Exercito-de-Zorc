#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recrutamento.h"
#include "grafo.h"

#define MAX_W 1000000  // Limite máximo de peso que pode ser carregado
#define MAX_D 10000    // Limite máximo de distância que pode ser percorrida
#define MAX_CAMINHO 100 // Tamanho máximo do caminho a ser percorrido

typedef struct {
    int habilidade;           // Habilidade total dos soldados recrutados
    int peso_usado;           // Peso total carregado até o momento
    int distancia_usada;      // Distância total percorrida
    int* caminho;             // Sequência de povos visitados
    int tamanho_caminho;      // Tamanho do caminho percorrido
    int* soldados_por_povo;   // Quantidade de soldados recrutados por povo
} EstadoDP;

EstadoDP* calcular_melhor_estado(Grafo* grafo, int atual, int peso_usado, int distancia_usada, int* visitado, EstadoDP** memo) {
    // Verifica se já existe um estado calculado para esse ponto
    if (memo[atual] != NULL) {
        // Cria uma cópia desse estado e retorna
        EstadoDP* estado = (EstadoDP*)malloc(sizeof(EstadoDP));
        estado->caminho = (int*)malloc(sizeof(int) * MAX_CAMINHO);
        estado->soldados_por_povo = (int*)calloc(grafo->quantidade_povos, sizeof(int));
        
        // Copia os dados do estado memoizado
        memcpy(estado->caminho, memo[atual]->caminho, sizeof(int) * memo[atual]->tamanho_caminho);
        memcpy(estado->soldados_por_povo, memo[atual]->soldados_por_povo, sizeof(int) * grafo->quantidade_povos);
        estado->tamanho_caminho = memo[atual]->tamanho_caminho;
        estado->habilidade = memo[atual]->habilidade;
        estado->peso_usado = memo[atual]->peso_usado;
        estado->distancia_usada = memo[atual]->distancia_usada;
        
        return estado;
    }

    // Cria um novo estado para essa posição
    EstadoDP* estado = (EstadoDP*)malloc(sizeof(EstadoDP));
    estado->caminho = (int*)malloc(sizeof(int) * MAX_CAMINHO);
    estado->soldados_por_povo = (int*)calloc(grafo->quantidade_povos, sizeof(int));
    estado->tamanho_caminho = 0;
    estado->habilidade = 0;
    estado->peso_usado = peso_usado;
    estado->distancia_usada = distancia_usada;

    visitado[atual] = 1; // Marca o povo atual como visitado
    estado->caminho[estado->tamanho_caminho++] = atual;

    // Tenta recrutar soldados no povo atual, se ainda há capacidade
    if (peso_usado < grafo->limite_peso) {
        int capacidade = grafo->limite_peso - peso_usado;
        int qtd_soldados = grafo->povos[atual].peso > 0 ? capacidade / grafo->povos[atual].peso : 0;
        estado->soldados_por_povo[atual] = qtd_soldados;
        estado->habilidade = qtd_soldados * grafo->povos[atual].habilidade;
        estado->peso_usado += qtd_soldados * grafo->povos[atual].peso;
    }

    // Procura entre os vizinhos aquele que permite aumentar mais a habilidade total
    int melhor_habilidade = 0;
    EstadoDP* melhor_estado = NULL;

    for (int i = 0; i < grafo->qtd_adj[atual]; i++) {
        int viz = grafo->adj[atual][i].destino;
        int dist = grafo->adj[atual][i].distancia;

        // Só visita vizinhos ainda não visitados e acessíveis dentro do limite de distância
        if (!visitado[viz] && distancia_usada + dist <= grafo->limite_distancia) {
            EstadoDP* estado_viz = calcular_melhor_estado(grafo, viz, estado->peso_usado, estado->distancia_usada + dist, visitado, memo);

            // Guarda o melhor resultado
            if (estado_viz != NULL && estado_viz->habilidade > melhor_habilidade) {
                if (melhor_estado != NULL) {
                    free(melhor_estado->caminho);
                    free(melhor_estado->soldados_por_povo);
                    free(melhor_estado);
                }
                melhor_estado = estado_viz;
                melhor_habilidade = estado_viz->habilidade;
            } else if (estado_viz != NULL) {
                free(estado_viz->caminho);
                free(estado_viz->soldados_por_povo);
                free(estado_viz);
            }
        }
    }
    
    // Se encontrou um caminho melhor, anexa ao estado atual
    if (melhor_estado != NULL) {
        estado->habilidade += melhor_estado->habilidade;
        estado->distancia_usada = melhor_estado->distancia_usada;
        
        for (int i = 0; i < melhor_estado->tamanho_caminho; i++) {
            estado->caminho[estado->tamanho_caminho++] = melhor_estado->caminho[i];
            estado->soldados_por_povo[melhor_estado->caminho[i]] = 
                melhor_estado->soldados_por_povo[melhor_estado->caminho[i]];
        }
        
        free(melhor_estado->caminho);
        free(melhor_estado->soldados_por_povo);
        free(melhor_estado);
    }

    // Cria uma cópia do estado para a memoização
    EstadoDP* estado_memo = (EstadoDP*)malloc(sizeof(EstadoDP));
    estado_memo->caminho = (int*)malloc(sizeof(int) * estado->tamanho_caminho);
    estado_memo->soldados_por_povo = (int*)malloc(sizeof(int) * grafo->quantidade_povos);
    
    memcpy(estado_memo->caminho, estado->caminho, sizeof(int) * estado->tamanho_caminho);
    memcpy(estado_memo->soldados_por_povo, estado->soldados_por_povo, sizeof(int) * grafo->quantidade_povos);
    estado_memo->tamanho_caminho = estado->tamanho_caminho;
    estado_memo->habilidade = estado->habilidade;
    estado_memo->peso_usado = estado->peso_usado;
    estado_memo->distancia_usada = estado->distancia_usada;

    memo[atual] = estado_memo;
    visitado[atual] = 0; // Marca como não visitado para permitir outras rotas

    return estado;
}

// Algoritmo principal usando programação dinâmica para encontrar o melhor caminho de recrutamento
ResultadoRecrutamento* solucao_dinamica(Grafo* grafo) {
    EstadoDP* melhor_estado = NULL;
    int melhor_habilidade = -1;

    // Tenta iniciar a busca a partir de cada povo
    for (int inicio = 0; inicio < grafo->quantidade_povos; inicio++) {
        EstadoDP** memo = (EstadoDP**)calloc(grafo->quantidade_povos, sizeof(EstadoDP*));  // Aloca nova memoização e array de visitados para cada tentativa
        int* visitado = (int*)calloc(grafo->quantidade_povos, sizeof(int));

        EstadoDP* estado = calcular_melhor_estado(grafo, inicio, 0, 0, visitado, memo);
        
        if (estado != NULL && estado->habilidade > melhor_habilidade) {
            if (melhor_estado != NULL) {
                free(melhor_estado->caminho);
                free(melhor_estado->soldados_por_povo);
                free(melhor_estado);
            }
            melhor_estado = estado;
            melhor_habilidade = estado->habilidade;
        } else if (estado != NULL) {
            free(estado->caminho);
            free(estado->soldados_por_povo);
            free(estado);
        }

        // Libera a memoização e o array de visitados
        for (int i = 0; i < grafo->quantidade_povos; i++) {
            if (memo[i] != NULL && memo[i] != melhor_estado) {
                free(memo[i]->caminho);
                free(memo[i]->soldados_por_povo);
                free(memo[i]);
            }
        }
        free(memo);
        free(visitado);
    }

    if (melhor_estado == NULL) {
        return NULL;
    }

    // Cria uma cópia dos dados do melhor estado
    ResultadoRecrutamento* res = (ResultadoRecrutamento*)malloc(sizeof(ResultadoRecrutamento));
    res->caminho = (int*)malloc(sizeof(int) * melhor_estado->tamanho_caminho);
    res->soldados_por_povo = (int*)malloc(sizeof(int) * grafo->quantidade_povos);
    
    // Copia os dados
    memcpy(res->caminho, melhor_estado->caminho, sizeof(int) * melhor_estado->tamanho_caminho);
    memcpy(res->soldados_por_povo, melhor_estado->soldados_por_povo, sizeof(int) * grafo->quantidade_povos);
    res->tamanho_caminho = melhor_estado->tamanho_caminho;
    res->habilidade_total = melhor_estado->habilidade;

    // Libera o melhor estado
    free(melhor_estado->caminho);
    free(melhor_estado->soldados_por_povo);
    free(melhor_estado);

    return res;
}

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


void imprimir_resultado(ResultadoRecrutamento* resultado, FILE* out) {
    // Imprime a habilidade total
    fprintf(out, "%d", resultado->habilidade_total);
    
    // Imprime cada povo visitado e sua quantidade de soldados
    for (int i = 0; i < resultado->tamanho_caminho; i++) {
        int p = resultado->caminho[i];
        // Imprime o povo e a quantidade de soldados (mesmo que seja 0)
        fprintf(out, " %d %d", p + 1, resultado->soldados_por_povo[p]);
    }
    fprintf(out, "\n");
}


void liberar_resultado(ResultadoRecrutamento* resultado) {
    free(resultado->caminho);
    free(resultado->soldados_por_povo);
    free(resultado);
}