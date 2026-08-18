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
} process;

#endif