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

void job_init(void);
int job_add(pid_t pid, const char *command);
process* job_find(int id);

#endif