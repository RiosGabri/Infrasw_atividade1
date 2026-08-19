#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "execution.h"
#include "task.h"

int execute_task(const char *nome) {
    Task *t = task_find(nome);
    if (t == NULL) {
        fprintf(stderr, "tarefa '%s' nao encontrada\n", nome);
        return -1;
    }
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork falhou");
        return -1;
    }
    if (pid == 0) {
        execvp(t->programa, t->args);
        perror("nao foi possivel executar o programa");
        _exit(127); 
    }
    int status;
    if (waitpid(pid, &status, 0) < 0) {
        perror("erro: waitpid falhou");
        return -1;
    }
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    return -1;
}

void run_sequential(char **nomes, int count) {
    for (int i = 0; i < count; i++) {
        int exit_code = execute_task(nomes[i]);
        if (exit_code != 0) {
            fprintf(stderr, "tarefa '%s' terminou com codigo %d\n",
                    nomes[i], exit_code);
        }
    }
}