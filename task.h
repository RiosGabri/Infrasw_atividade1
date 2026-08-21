#ifndef TASK_H
#define TASK_H

#define MAX_NOME 64
#define MAX_ARGS 64
#define MAX_PATH 256
#define MAX_TASKS 100

typedef struct Task{
    char nome[MAX_NOME];
    char programa[MAX_PATH];
    char *args[MAX_ARGS];
    char input_file[MAX_PATH];  
    char output_file[MAX_PATH]; 
    int append_mode;
    int uso;
} Task;

void task_init(void);
int task_add(const char *nome, const char *programa, char **args, int argc);
Task* task_find(const char *noome);
int task_set_input(const char *nome, const char *filename);
int task_set_output(const char *nome, const char *filename, int append_mode);
#endif