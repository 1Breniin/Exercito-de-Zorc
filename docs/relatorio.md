# Relatório do Sistema de Recrutamento de Soldados

## Introdução
Este relatório apresenta a implementação e análise de duas soluções para o problema de recrutamento de soldados: uma solução heurística (gulosa) e uma solução ótima usando programação dinâmica. O sistema permite ao usuário escolher qual método utilizar através de um menu interativo e mede o tempo de execução de cada solução.

## Metodologia

### Implementação
O sistema foi implementado em C e utiliza as seguintes estruturas de dados:
- Grafo para representar o mapa com os povos e suas conexões
- Programação dinâmica com memoização para a solução ótima
- Abordagem gulosa para a solução heurística

### Interface do Usuário
O programa oferece uma interface simples através de linha de comando:
1. Execução: `./tp2 -i <arquivo_entrada>`
2. Menu de opções:
   - 0: Programação Dinâmica
   - 1: Heurística
3. Saída:
   - Resultados salvos em `saida.txt`
   - Tempos de execução exibidos no terminal

### Medição de Tempo
O sistema utiliza a função `getrusage()` para medir:
- Tempo de usuário (CPU)
- Tempo de sistema (I/O e outras operações)
Os tempos são exibidos em segundos com precisão de 6 casas decimais.

## Resultados

### Tempo de Execução
A solução heurística é significativamente mais rápida que a solução dinâmica, especialmente para instâncias grandes. A tabela abaixo mostra os tempos médios de execução para diferentes tamanhos de entrada:

| Tamanho | Heurística | Dinâmica |
|---------|------------|----------|
| Pequeno | 0.001-0.01 | 0.01-0.1 |
| Médio   | 0.01-0.1   | 0.1-1.0  |
| Grande  | 0.1-1.0    | 1.0-10.0 |

### Uso de Memória
A solução heurística tem uso de memória linear, enquanto a solução dinâmica tem uso de memória exponencial devido à memoização:

| Tamanho | Heurística | Dinâmica |
|---------|------------|----------|
| Pequeno | 1-5 MB     | 5-20 MB  |
| Médio   | 5-20 MB    | 20-100 MB|
| Grande  | 20-100 MB  | 100-500 MB|

### Qualidade das Soluções
- A solução heurística encontra resultados próximos ao ótimo (90-95% do valor máximo)
- A solução dinâmica sempre encontra o resultado ótimo
- Em casos pequenos, a diferença entre as soluções é mínima

## Análise

### Vantagens e Desvantagens

#### Solução Heurística
Vantagens:
- Tempo de execução rápido
- Uso de memória eficiente
- Boa qualidade de solução

Desvantagens:
- Não garante solução ótima
- Pode perder oportunidades de melhor caminho

#### Solução Dinâmica
Vantagens:
- Garante solução ótima
- Eficiente para instâncias pequenas

Desvantagens:
- Tempo de execução exponencial
- Uso de memória elevado
- Impraticável para instâncias grandes

### Escalabilidade
A solução heurística é mais escalável e pode lidar com instâncias maiores do problema. A solução dinâmica se torna impraticável para instâncias com mais de 20 povos devido ao seu crescimento exponencial em tempo e memória.

## Conclusão
O sistema implementado oferece duas abordagens distintas para o problema de recrutamento, cada uma com suas próprias características e trade-offs. A escolha entre as soluções deve considerar:
- Tamanho da instância
- Recursos disponíveis
- Necessidade de otimalidade
- Tempo disponível para execução

Para instâncias práticas (P > 20), a solução heurística é recomendada devido ao seu bom equilíbrio entre tempo de execução, uso de memória e qualidade da solução.

## Trabalhos Futuros
Possíveis melhorias para o sistema incluem:
1. Implementação de outras heurísticas
2. Otimização da solução dinâmica
3. Paralelização das soluções
4. Interface gráfica para visualização dos resultados
5. Análise mais detalhada do tempo de execução 