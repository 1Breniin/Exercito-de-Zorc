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
P W D
p1 h1
p2 h2
...
pP hP
C
v1 u1 d1
v2 u2 d2
...
vC uC dC
```
Onde:
- P: número de povos
- W: peso máximo que pode ser carregado
- D: distância máxima que pode ser percorrida
- pi: peso de cada soldado do povo i
- hi: habilidade de cada soldado do povo i
- C: número de conexões entre povos
- vi, ui: povos conectados
- di: distância entre os povos

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