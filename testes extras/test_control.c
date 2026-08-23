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
    int id1 = processo_add(p1, "comando_lento");

    pid_t p2 = fork();
    if (p2 == 0) {
        exit(5);
    }
    int id2 = processo_add(p2, "comando_rapido");

    sleep(1); 

    printf("Verificacao 1 (Processo rapido deve ter concluido, lento deve estar rodando):\n");
    processo_print_all();

    printf("\nAguardando o processo lento finalizar usando processo_wait(%d)...\n", id1);
    processo_wait(id1);

    printf("\nVerificacao 2 (Testando dar wait em processo ja concluido):\n");
    processo_wait(id2);

    printf("\nVerificacao 3 (Não deve haver mais nada rodando):\n");
    processo_print_all();

    return 0;
}