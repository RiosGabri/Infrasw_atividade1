#include <stdio.h>
#include <unistd.h>
#include "task.h"
#include "control.h"
#include "execution.h"

int main(void) {
    printf("--- TESTE ISOLADO DO EXECUTION.C ---\n\n");
    task_init();
    processo_init();

    char *args[] = {"/bin/sleep", "2", NULL};
    task_add("dormir", "/bin/sleep", args, 2);

    printf("1. Executando tarefa 'dormir' em background...\n");
    run_background("dormir");

    printf("\n2. Verificando se foi adicionada aos processos (jobs):\n");
    processo_print_all();

    printf("\n3. Aguardando 3 segundos...\n");
    sleep(3);
    
    printf("\n4. Verificando status apos a conclusao:\n");
    processo_print_all();

    return 0;
}