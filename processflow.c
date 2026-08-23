#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "task.h"
#include "control.h"
#include "execution.h"

static void process_line(char *line) {
    parsedLine cmd;
    if (!parse(line, &cmd) || cmd.argc == 0) return;
    const char *nome_cmd = cmd.argv[0];
    if (strcmp(nome_cmd, "exit") == 0) {
        exit(0);
    }
    else if (strcmp(nome_cmd, "task") == 0) {
        if (cmd.argc < 3) {
            fprintf(stderr, "erro: uso: task <nome> <programa> [argumentos...]\n");
        } else {
            task_add(cmd.argv[1], cmd.argv[2], &cmd.argv[2], cmd.argc - 2);
        }
    }
    else if (strcmp(nome_cmd, "run") == 0) {
        if (cmd.argc < 3) {
            fprintf(stderr, "erro: uso: run <sequential|parallel|pipe> <tarefa...>\n");
        } else {
            int num_tarefas = cmd.argc - 2;
            const char *modo = cmd.argv[1];
            if (strcmp(modo, "sequential") == 0) run_sequential(&cmd.argv[2], num_tarefas);
            else if (strcmp(modo, "parallel") == 0) run_parallel(&cmd.argv[2], num_tarefas);
            else if (strcmp(modo, "pipe") == 0) run_pipe(&cmd.argv[2], num_tarefas);
            else fprintf(stderr, "erro: modo de run desconhecido: %s\n", modo);
        }
    }
    else if (strcmp(nome_cmd, "input") == 0) {
        if (cmd.argc != 3) {
            fprintf(stderr, "erro: uso: input <tarefa> <arquivo>\n");
        } else if (task_set_input(cmd.argv[1], cmd.argv[2]) != 0) {
            fprintf(stderr, "erro: tarefa '%s' nao encontrada\n", cmd.argv[1]);
        }
    }
    else if (strcmp(nome_cmd, "output") == 0) {
        if (cmd.argc != 3) {
            fprintf(stderr, "erro: uso: output <tarefa> <arquivo>\n");
        } else if (task_set_output(cmd.argv[1], cmd.argv[2], 0) != 0) {
            fprintf(stderr, "erro: tarefa '%s' nao encontrada\n", cmd.argv[1]);
        }
    }
    else if (strcmp(nome_cmd, "append") == 0) {
        if (cmd.argc != 3) {
            fprintf(stderr, "erro: uso: append <tarefa> <arquivo>\n");
        } else if (task_set_output(cmd.argv[1], cmd.argv[2], 1) != 0) {
            fprintf(stderr, "erro: tarefa '%s' nao encontrada\n", cmd.argv[1]);
        }
    }
    else if (strcmp(nome_cmd, "jobs") == 0) {
        processo_print_all();
    }
    else if (strcmp(nome_cmd, "workdir") == 0) {
        if (cmd.argc != 2) {
            fprintf(stderr, "erro: uso: workdir <diretorio>\n");
        } else if (chdir(cmd.argv[1]) != 0) {
            perror("erro: nao foi possivel mudar de diretorio");
        }
    }
    else if (strcmp(nome_cmd, "start") == 0) {
        if (cmd.argc != 2) {
            fprintf(stderr, "erro: uso: start <tarefa>\n");
        } else {
            run_background(cmd.argv[1]);
        }
    }
    else if (strcmp(nome_cmd, "wait") == 0) {
        if (cmd.argc != 2) {
            fprintf(stderr, "erro: uso: wait <jobId>\n");
        } else {
            char *endptr;
            long id = strtol(cmd.argv[1], &endptr, 10);
            if (*endptr != '\0' || endptr == cmd.argv[1]) {
                fprintf(stderr, "erro: jobId invalido: %s\n", cmd.argv[1]);
            } else if (processo_wait((int)id) < 0) {
                fprintf(stderr, "erro: job %ld nao encontrado\n", id);
            }
        }
    }
    else {
        fprintf(stderr, "erro: comando desconhecido: %s\n", nome_cmd);
    }
}

static void run_interactive(void) {
    char *line = NULL;
    size_t len = 0;
    
    while (1) {
        printf("processflow> ");
        if (getline(&line, &len, stdin) == -1) break; 
        process_line(line);
    }
    free(line);
}

static void run_workflow(const char *filepath) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        fprintf(stderr, "nao foi possivel abrir %s\n", filepath);
        exit(1); 
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while ((nread = getline(&line, &len, fp)) != -1) {
        printf("%s", line); 
        if (nread == 0 || line[nread - 1] != '\n') printf("\n");
        process_line(line);
    }
    fprintf(stderr, "aviso: workflow terminou sem comando exit\n");

    free(line);
    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "uso: %s [workflowFile]\n", argv[0]);
        return 1; 
    }
    task_init();
    processo_init();
 
    if (argc == 2) {
        run_workflow(argv[1]); 
    } else {
        run_interactive();
    }

    return 0;
}