#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "utils.h"

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

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "-i") != 0) {
        imprimir_uso();
        return 1;
    }

    struct rusage inicio, fim;
    getrusage(RUSAGE_SELF, &inicio);

    Instancias* instancias = ler_entrada(argv[2]);
    if (instancias == NULL) {
        printf("Erro ao ler o arquivo de entrada: %s\n", argv[2]);
        return 1;
    }

    escrever_saida("saida.txt", instancias);
    liberar_instancias(instancias);

    getrusage(RUSAGE_SELF, &fim);
    imprimir_tempos(inicio, fim);

    return 0;
}
