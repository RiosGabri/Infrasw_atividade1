#include <stdio.h>
#include <string.h>
#include "control.h"

#define MAX_PROCESS 100

static process processo[MAX_PROCESS];
static int next_id = 1;

void job_init(void) {
    for (int i = 0; i < MAX_PROCESS; i++) {
        processo[i].uso = 0;
    }
}

int job_add(pid_t pid, const char *command) {
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (!processo[i].uso) {
            processo[i].ID = next_id++;
            processo[i].pid = pid;
            strncpy(processo[i].command, command, 255);
            processo[i].estado = funcionando;
            processo[i].uso = 1;
            return processo[i].ID;
        }
    }
    return -1;
}