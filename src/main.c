#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "utils.h"
#include "recrutamento.h"

void imprimir_tempos(struct rusage inicio, struct rusage fim) {
    long utime_sec = fim.ru_utime.tv_sec - inicio.ru_utime.tv_sec;
    long utime_usec = fim.ru_utime.tv_usec - inicio.ru_utime.tv_usec;
    long stime_sec = fim.ru_stime.tv_sec - inicio.ru_stime.tv_sec;
    long stime_usec = fim.ru_stime.tv_usec - inicio.ru_stime.tv_usec;

    double tempo_usuario = utime_sec + utime_usec / 1e6;
    double tempo_sistema = stime_sec + stime_usec / 1e6;

    printf("Tempo de usuário: %.6f segundos\n", tempo_usuario);
    printf("Tempo de sistema: %.6f segundos\n", tempo_sistema);
}

void imprimir_uso() {
    printf("Uso: ./tp2 -i <arquivo_entrada>\n");
    printf("  -i: arquivo de entrada contendo as instâncias do problema\n");
}

void imprimir_menu() {
    printf("\nEscolha o método de solução:\n");
    printf("0 - Programação Dinâmica\n");
    printf("1 - Heurística\n");
    printf("Sua escolha: ");
}

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "-i") != 0) {
        imprimir_uso();
        return 1;
    }

    Instancias* instancias = ler_entrada(argv[2]);
    if (instancias == NULL) {
        printf("Erro ao ler o arquivo de entrada: %s\n", argv[2]);
        return 1;
    }

    imprimir_menu();
    int escolha;
    if (scanf("%d", &escolha) != 1) {
        printf("Erro ao ler a escolha\n");
        liberar_instancias(instancias);
        return 1;
    }

    struct rusage inicio, fim;
    getrusage(RUSAGE_SELF, &inicio);

    FILE* saida = fopen("saida.txt", "w");
    if (saida == NULL) {
        printf("Erro ao abrir arquivo de saída\n");
        liberar_instancias(instancias);
        return 1;
    }

    for (int i = 0; i < instancias->total_instancias; i++) {
        ResultadoRecrutamento* resultado;
        
        if (escolha == 0) {
            resultado = solucao_dinamica(instancias->instancias[i]);
        } else if (escolha == 1) {
            resultado = solucao_heuristica(instancias->instancias[i]);
        } else {
            printf("Escolha inválida!\n");
            fclose(saida);
            liberar_instancias(instancias);
            return 1;
        }

        if (resultado != NULL) {
            imprimir_resultado(resultado, saida);
            liberar_resultado(resultado);
        }
    }

    fclose(saida);
    liberar_instancias(instancias);

    getrusage(RUSAGE_SELF, &fim);
    printf("\nTempos de execução:\n");
    imprimir_tempos(inicio, fim);

    return 0;
}