#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "task.h"
#include "control.h"
#include "execution.h"

static void process_line(char *line) {
    parsedLine cmd;
    if (!parse(line, &cmd) || cmd.argc == 0) return;
    if (strcmp(cmd.argv[0], "exit") == 0) {
        exit(0);
    }
    else if (strcmp(cmd.argv[0], "task") == 0 && cmd.argc >= 3) {
        task_add(cmd.argv[1], cmd.argv[2], &cmd.argv[2], cmd.argc - 2);
    }
    else if (strcmp(cmd.argv[0], "run") == 0 && cmd.argc >= 3) {
        int num_tarefas = cmd.argc - 2;
        if (strcmp(cmd.argv[1], "sequential") == 0) run_sequential(&cmd.argv[2], num_tarefas);
        else if (strcmp(cmd.argv[1], "parallel") == 0) run_parallel(&cmd.argv[2], num_tarefas);
        else if (strcmp(cmd.argv[1], "pipe") == 0) run_pipe(&cmd.argv[2], num_tarefas);
    }
    else if (strcmp(cmd.argv[0], "input") == 0 && cmd.argc == 3) {
        task_set_input(cmd.argv[1], cmd.argv[2]);
    }
    else if (strcmp(cmd.argv[0], "output") == 0 && cmd.argc == 3) {
        task_set_output(cmd.argv[1], cmd.argv[2], 0);
    }
    else if (strcmp(cmd.argv[0], "append") == 0 && cmd.argc == 3) {
        task_set_output(cmd.argv[1], cmd.argv[2], 1);
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
    while (getline(&line, &len, fp) != -1) {
        process_line(line);
    }
    free(line);
    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "uso: %s [workflowFile]\n", argv[0]);
        return 1; 
    }
    task_init();
    job_init();
 
    if (argc == 2) {
        run_workflow(argv[1]); 
    } else {
        run_interactive();
    }

    return 0;
}