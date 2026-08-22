#ifndef CONTROL_H
#define CONTROL_H

#include <sys/types.h>

typedef enum{
    falha,
    funcionando,
    finalizado
} state;

typedef struct {
    int ID;
    pid_t pid;
    char command[256];
    state estado;
    int saida;
    int uso;
} process;

void processo_init(void);
int processo_add(pid_t pid, const char *command);
process* processo_find(int id);
void processo_print_all(void);

#endif