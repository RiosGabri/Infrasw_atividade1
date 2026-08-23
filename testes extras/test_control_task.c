#include <stdio.h>
#include <stdlib.h>
#include "control.h"
#include "task.h"
#include "execution.h"
#include "parser.h"

int main(void) {
    task_init();
    processo_init();

    printf("--- TESTES DE TASK E JOB ---\n");
    // TESTE 1 -> task_add: caso normal 
    char *args1[] = {"/bin/ls", "-l", NULL};
    int r1 = task_add("listar", "/bin/ls", args1, 2);
    printf("task_add(listar) -> %d (esperado 0)\n", r1);

    // TESTE 2 -> deve achar 
    Task *t = task_find("listar");
    printf("task_find(listar) -> %s (esperado != NULL), programa=%s\n",
           t ? "achou" : "NULL", t ? t->programa : "-");

    // TESTE 3 -> nome duplicado
    int r2 = task_add("listar", "/bin/ls", args1, 2);
    printf("task_add(listar) duplicado -> %d (esperado -1)\n", r2);

    /* TESTE 4 -> nome inexistente */
    Task *t2 = task_find("naoexiste");
    printf("task_find(naoexiste) -> %s (esperado NULL)\n", t2 ? "achou (ERRO)" : "NULL");

    // TESTE 5 -> processo_add + processo_find 
    int jid = processo_add(1234, "listar");
    printf("processo_add -> ID=%d (esperado 1)\n", jid);

    process *p = processo_find(jid);
    printf("processo_find(%d) -> %s", jid, p ? "achou" : "NULL");
    if (p) {
        printf(", pid=%d, command=%s, estado=%d, saida=%d (esperado saida=-1)\n",
               p->pid, p->command, p->estado, p->saida);
    } else {
        printf("\n");
    }

    // TESTE 6 ->id inexistente 
    process *p2 = processo_find(999);
    printf("processo_find(999) -> %s (esperado NULL)\n", p2 ? "achou (ERRO)" : "NULL");

    printf("\n--- TESTES DO PARSER ---\n");
    // TESTE 7 -> Testando a separação de strings
    char linha_teste[] = "  task   echo   /bin/echo ola mundo  \n";
    parsedLine p_line;
    int parsed = parse(linha_teste, &p_line);
    printf("parse() -> retornou %d (esperado 1)\n", parsed);
    printf("argc = %d (esperado 5)\n", p_line.argc);
    if (parsed) {
        for (int i = 0; i < p_line.argc; i++) {
            printf("argv[%d] = '%s'\n", i, p_line.argv[i]);
        }
    }

    printf("\n--- TESTES DE EXECUCAO ---\n");
    // TESTE 8 -> execute_task isolado
    printf("Executando a tarefa 'listar' (execute_task):\n");
    int exec_code = execute_task("listar");
    printf("\nexecute_task() finalizado com codigo: %d (esperado 0)\n", exec_code);

    // TESTE 9 -> run_sequential
    char *args2[] = {"/bin/echo", "Testando o comando echo", NULL};
    task_add("falar", "/bin/echo", args2, 2);
    
    printf("\nExecutando tarefas em sequencia (run_sequential 'listar' e 'falar'):\n");
    char *seq_tasks[] = {"listar", "falar"};
    run_sequential(seq_tasks, 2);
    printf("\nrun_sequential finalizado.\n");

    printf("\n--- TESTES DA ETAPA 4 (PARALLEL E PIPE) ---\n");

    // Cadastrando tarefas extras para os testes
    char *args_echo[] = {"/bin/echo", "ola mundo pipe", NULL};
    task_add("falar_pipe", "/bin/echo", args_echo, 2);
    
    char *args_wc[] = {"/usr/bin/wc", "-w", NULL};
    task_add("contar", "/usr/bin/wc", args_wc, 2);

    // TESTE 10 -> run_parallel
    printf("Executando tarefas em paralelo (run_parallel 'listar' e 'falar_pipe'):\n");
    char *par_tasks[] = {"listar", "falar_pipe"};
    run_parallel(par_tasks, 2);
    printf("run_parallel finalizado.\n");

    // TESTE 11 -> run_pipe
    printf("\nExecutando tarefas com pipe (run_pipe 'falar_pipe' | 'contar'):\n");
    // O echo vai mandar "ola mundo pipe" (3 palavras) e o wc -w vai contar. A saída na tela deve ser "3".
    char *pipe_tasks[] = {"falar_pipe", "contar"};
    run_pipe(pipe_tasks, 2);
    printf("run_pipe finalizado.\n");

    return 0;
}