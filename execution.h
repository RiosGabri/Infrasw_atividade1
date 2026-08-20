#ifndef EXECUTION_H
#define EXECUTION_H
#include <sys/types.h>
#include "task.h"

pid_t executar_task(const char *nome);
int wait_task(pid_t pid);
int execute_task(const char *nome);
void run_sequential(char **nomes, int count);
void run_parallel(char **nomes, int count);
void run_pipe(char **nomes, int count);

#endif