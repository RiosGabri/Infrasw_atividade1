#ifndef PARSER_H
#define PARSER_H
#define MAX_ARGS 64

typedef struct {
    int argc;
    char *argv[MAX_ARGS];
}parsedLine;

int parse(char *line, parsedLine *linha);
#endif