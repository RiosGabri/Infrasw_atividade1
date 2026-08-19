#include "parser.h"
#include <string.h>

int parse(char *line, parsedLine *linha){
    linha->argc = 0;
    char *token = strtok(line, " \t\n");
    while(token != NULL && linha->argc < MAX_ARGS - 1){
        linha->argv[linha->argc++] = token;
        token = strtok(NULL, " \t\n");
    }
    linha->argv[linha->argc] = NULL;
    return linha->argc > 0;
}