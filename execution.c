#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include "execution.h"
#include "task.h"
#include "control.h"

pid_t executar_task(const char *nome) {
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
        if (t->input_file[0] != '\0') {
            int fd_in = open(t->input_file, O_RDONLY);
            if (fd_in >= 0) {
                dup2(fd_in, STDIN_FILENO);
                close(fd_in);
            } else {
                perror("erro ao abrir arquivo de entrada");
                _exit(1);
            }
        }
        if (t->output_file[0] != '\0') {
            int flags = O_WRONLY | O_CREAT | (t->append_mode ? O_APPEND : O_TRUNC);
            int fd_out = open(t->output_file, flags, 0644);
            if (fd_out >= 0) {
                dup2(fd_out, STDOUT_FILENO);
                close(fd_out);
            } else {
                perror("erro ao abrir arquivo de saida");
                _exit(1);
            }
        }
        execvp(t->programa, t->args);
        perror("nao foi possivel executar o programa");
        _exit(127);
    }
    return pid;
}

int wait_task(pid_t pid) {
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

int execute_task(const char *nome) {
    pid_t pid = executar_task(nome);
    if (pid < 0) {
        return -1;
    }
    return wait_task(pid);
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

void run_parallel(char **nomes, int count) {
    pid_t pids[count];
    char *validos[count];
    int n_validos = 0;
    for (int i = 0; i < count; i++) {
        pid_t pid = executar_task(nomes[i]);
        if (pid > 0) {
            pids[n_validos] = pid;
            validos[n_validos] = nomes[i];
            n_validos++;
        }
    }
    for (int i = 0; i < n_validos; i++) {
        int exit_code = wait_task(pids[i]);
        if (exit_code != 0) {
            fprintf(stderr, "tarefa '%s' (paralela) terminou com codigo %d\n",
                    validos[i], exit_code);
        }
    }
}

void run_pipe(char **nomes, int count) {
    if (count < 2) {
        fprintf(stderr, "pipe precisa de pelo menos 2 tarefas\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        if (task_find(nomes[i]) == NULL) {
            fprintf(stderr, "tarefa '%s' nao encontrada -> pipe cancelado, nenhum processo criado\n",
                    nomes[i]);
            return;
        }
    }
    int n_pipes = count - 1;
    int pipefds[n_pipes][2];
    for (int i = 0; i < n_pipes; i++) {
        if (pipe(pipefds[i]) < 0) {
            perror("erro: pipe() falhou");
            for (int j = 0; j < i; j++) {
                close(pipefds[j][0]);
                close(pipefds[j][1]);
            }
            return;
        }
    }
    pid_t pids[count];
    for (int i = 0; i < count; i++) {
        Task *t = task_find(nomes[i]);
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork falhou");
            pids[i] = -1;
            continue;
        }
        if (pid == 0) {
            if (i > 0) {
                dup2(pipefds[i - 1][0], STDIN_FILENO);
            }
            if (i < n_pipes) {
                dup2(pipefds[i][1], STDOUT_FILENO);
            }
            for (int j = 0; j < n_pipes; j++) {
                close(pipefds[j][0]);
                close(pipefds[j][1]);
            }
            execvp(t->programa, t->args);
            perror("nao foi possivel executar o programa");
            _exit(127);
        }
        pids[i] = pid;
    }
    for (int i = 0; i < n_pipes; i++) {
        close(pipefds[i][0]);
        close(pipefds[i][1]);
    }
    for (int i = 0; i < count; i++) {
        if (pids[i] < 0) continue;
        int exit_code = wait_task(pids[i]);
        if (exit_code != 0) {
            fprintf(stderr, "tarefa '%s' (pipe) terminou com codigo %d\n",
                    nomes[i], exit_code);
        }
    }
}

void run_background(const char *nome) {
    pid_t pid = executar_task(nome);
    if (pid > 0) {
        int id = processo_add(pid, nome);
        printf("[%d] %d\n", id, pid);
    }
}