#include <stdio.h>
#include <string.h>
#include "control.h"
#include <sys/wait.h>

#define MAX_PROCESS 100

static process processo[MAX_PROCESS];
static int next_id = 1;

void processo_init(void) {
    for (int i = 0; i < MAX_PROCESS; i++) {
        processo[i].uso = 0;
    }
}

int processo_add(pid_t pid, const char *command) {
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (!processo[i].uso) {
            processo[i].ID = next_id++;
            processo[i].pid = pid;
            strncpy(processo[i].command, command, 255);
            processo[i].command[255] = '\0';
            processo[i].estado = funcionando;
            processo[i].saida = -1; 
            processo[i].uso = 1;
            return processo[i].ID;
        }
    }
    return -1;
}

process* processo_find(int id) {
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (processo[i].uso && processo[i].ID == id) {
            return &processo[i];
        }
    }
    return NULL;
}

void processo_print_all(void) {
    int ativos = 0;
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (processo[i].uso && processo[i].estado == funcionando) {
            int status;
            pid_t res = waitpid(processo[i].pid, &status, WNOHANG);
            if (res > 0) { 
                processo[i].estado = finalizado;
                processo[i].saida = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
                printf("[%d] PID %d - %s (Concluido, saida: %d)\n", 
                       processo[i].ID, processo[i].pid, processo[i].command, processo[i].saida);
            } else if (res == 0) { 
                ativos++;
                printf("[%d] PID %d - %s (Rodando)\n", 
                       processo[i].ID, processo[i].pid, processo[i].command);
            }
        }
    }
    if (ativos == 0) printf("Nenhum processo em background rodando.\n");
}