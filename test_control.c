#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "control.h"

int main(void) {
    printf("--- TESTE ISOLADO DO CONTROL.C ---\n\n");
    processo_init();
    pid_t p1 = fork();
    if (p1 == 0) {
        sleep(2);
        exit(0);
    }
    processo_add(p1, "comando_lento");

    pid_t p2 = fork();
    if (p2 == 0) {
        exit(5);
    }
    processo_add(p2, "comando_rapido");

    sleep(1);

    printf("Verificacao 1 (Processo rápido deve ter concluido, lento deve estar rodando):\n");
    processo_print_all();

    printf("\nAguardando o processo lento finalizar...\n");
    sleep(2);

    printf("\nVerificacao 2 (Processo lento deve ter concluido agora):\n");
    processo_print_all();

    printf("\nVerificacao 3 (Não deve haver mais nada rodando):\n");
    processo_print_all();

    return 0;
}