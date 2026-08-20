#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "task.h"

static Task tasks[MAX_TASKS];
//static int task_count = 0;

void task_init(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].uso = 0;
    }
}

Task* task_find(const char *name) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].uso && strcmp(tasks[i].nome, name) == 0) {
            return &tasks[i];
        }
    }
    return NULL;
}

int task_add(const char *name, const char *program, char **args, int argc) {
    if (task_find(name) != NULL) {
        return -1;
    }
    for (int i = 0; i < MAX_TASKS; i++) {
        if (!tasks[i].uso) {
            strncpy(tasks[i].nome, name, MAX_NOME - 1);
            tasks[i].nome[MAX_NOME - 1] = '\0';
            strncpy(tasks[i].programa, program, MAX_PATH - 1);
            tasks[i].programa[MAX_PATH - 1] = '\0';
            for (int j = 0; j < argc && j < MAX_ARGS - 1; j++) {
                tasks[i].args[j] = strdup(args[j]);
            }
            tasks[i].args[argc] = NULL;
            tasks[i].input_file[0] = '\0';
            tasks[i].output_file[0] = '\0';
            tasks[i].append_mode = 0;
            tasks[i].uso = 1;
            return 0;
        }
    }
    return -1;
}