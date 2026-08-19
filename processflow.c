#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "task.h"
#include "control.h"


int main(int argc, char *argv[]) {
        if (argc > 2) {
        fprintf(stderr, "uso: %s [workflowFile]\n", argv[0]);
        return 1; 
    }
    task_init();
    job_init();
 
    if (argc == 2) {
        //workflow;
    } else {
        //interactive;
    }

    return 0;
}