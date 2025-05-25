#ifndef RECRUTAMENTO_H
#define RECRUTAMENTO_H

#include "grafo.h"

/**
 * @brief Estrutura que armazena o resultado do recrutamento
 * 
 * Esta estrutura contém todas as informações necessárias sobre o recrutamento
 * realizado, incluindo o caminho percorrido, quantidade de soldados recrutados
 * por povo e a habilidade total da tropa.
 */
typedef struct {
    int* caminho;              // Array com a sequência de povos visitados
    int* soldados_por_povo;    // Quantidade de soldados recrutados em cada povo
    int tamanho_caminho;       // Tamanho do caminho percorrido
    int habilidade_total;      // Soma total das habilidades dos soldados recrutados
} ResultadoRecrutamento;

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
ResultadoRecrutamento* solucao_heuristica(Grafo* grafo);

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
ResultadoRecrutamento* solucao_dinamica(Grafo* grafo);

/**
 * @brief Imprime o resultado do recrutamento em um arquivo
 * 
 * @param resultado Resultado do recrutamento a ser impresso
 * @param out Arquivo onde o resultado será impresso
 */
void imprimir_resultado(ResultadoRecrutamento* resultado, FILE* out);

/**
 * @brief Libera a memória alocada para o resultado do recrutamento
 * 
 * @param resultado Resultado do recrutamento a ser liberado
 */
void liberar_resultado(ResultadoRecrutamento* resultado);

#endif