#ifdef TASK_H
#define TASK_H

#define MAX_NOME 64
#define MAX_ARGS 64
#define MAX_PATH 256

typedef struct Task{
    char nome[MAX_NOME];
    char programa[MAX_PATH];
    char *args[MAX_ARGS];
} Task;

#endif 