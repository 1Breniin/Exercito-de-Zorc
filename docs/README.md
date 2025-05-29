# Sistema de Recrutamento de Soldados

## Descrição
Este sistema implementa duas soluções para o problema de recrutamento de soldados em um mundo fictício: uma solução heurística (gulosa) e uma solução ótima usando programação dinâmica. O programa permite ao usuário escolher qual método utilizar através de um menu interativo e mede o tempo de execução de cada solução.

## Compilação
Para compilar o projeto, execute:
```bash
make
```

## Execução
Para executar o programa:
```bash
./tp2 -i <arquivo_entrada>
```

O programa exibirá um menu com as seguintes opções:
- 0: Programação Dinâmica
- 1: Heurística

Após a execução, os resultados serão salvos em `saida.txt` e os tempos de execução serão exibidos no terminal.

## Estrutura do Projeto
```
.
├── src/
│   ├── main.c
│   ├── grafo.c
│   ├── grafo.h
│   ├── recrutamento.c
│   └── recrutamento.h
├── docs/
│   ├── README.md
│   └── documentacao.md
└── Makefile
```

## Formato de Entrada
O arquivo de entrada deve seguir o formato:
```
K
P D W C
p1 w1 h1
p2 w2 h2
...
pP wP hP
a1 b1 d1
a2 b2 d2
...
aC bC dC
(repetir a estrutura acima para cada uma das K instâncias)
```
Onde:
- `K`: número de instâncias no arquivo.
- `P`: número de povos.
- `D`: distância máxima (em metros) que Zork pode andar.
- `W`: peso da nave em gramas.
- `C`: número de caminhos existentes entre os povos.
- `pi`: identificador do povo `i` (inteiro de 1 até P).
- `wi`: peso de cada soldado do povo `i` (em gramas).
- `hi`: habilidade dos soldados do povo `i`, valor de 1 (pior) a 10 (melhor).
- `ai`, `bi`: identificadores dos dois povos conectados pelo caminho `i`.
- `di`: distância entre os povos `ai` e `bi` (em metros).

### Exemplo:

2
6 10 310 7
1 70 2
2 100 3
3 20 7
4 90 4
5 20 3
6 10 1
1 2 3
1 5 2
2 3 4
2 4 2
3 6 3
4 5 3
4 6 5
5 6 317 6
1 10 10
2 30 9
3 1 1
4 3 6
5 2 4
1 2 3
1 3 2
1 4 2
2 3 1
3 5 3
4 5 4

## Formato de Saída
O arquivo de saída (`saida.txt`) conterá:
```
N
p1 p2 ... pN
s1 s2 ... sN
H
```
Onde:
- N: número de povos visitados
- pi: sequência de povos visitados
- si: quantidade de soldados recrutados em cada povo
- H: soma total das habilidades dos soldados recrutados

## Medição de Tempo
O sistema mede e exibe:
- Tempo de usuário (CPU)
- Tempo de sistema (I/O e outras operações)

Os tempos são exibidos em segundos com precisão de 6 casas decimais.

## Documentação
Para mais detalhes sobre a implementação e análise de resultados, consulte:
- `docs/documentacao.md`: Documentação técnica detalhada
- `docs/README.md`: Este arquivo

## Limpeza
Para limpar os arquivos compilados:
```bash
make clean
``` 