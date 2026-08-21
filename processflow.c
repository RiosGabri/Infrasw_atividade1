#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "task.h"
#include "control.h"
#include "execution.h"

static void run_interactive(void) {
    //char *line = NULL;
    //size_t len = 0;
    //parsedLine cmd;
}

static void run_workflow(const char *filepath) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        fprintf(stderr, "nao foi possivel abrir %s\n", filepath);
        exit(1); 
    }
    //char *line = NULL;
    //size_t len = 0;
    //parsedLine cmd;
    //ssize_t nread;
}




int main(int argc, char *argv[]) {
        if (argc > 2) {
        fprintf(stderr, "uso: %s [workflowFile]\n", argv[0]);
        return 1; 
    }
    task_init();
    job_init();
 
    if (argc == 2) {
        run_workflow();
    } else {
        run_interactive();
    }

    return 0;
}