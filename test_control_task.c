#include <stdio.h>
#include "control.h"
#include "task.h"

int main(void) {
    task_init();
    job_init();

    //TESTE 1 -> task_add: caso normal 
    char *args1[] = {"/bin/ls", "-l", NULL};
    int r1 = task_add("listar", "/bin/ls", args1, 2);
    printf("task_add(listar) -> %d (esperado 0)\n", r1);

    //TESTE 2 -> deve achar 
    Task *t = task_find("listar");
    printf("task_find(listar) -> %s (esperado != NULL), programa=%s\n",
           t ? "achou" : "NULL", t ? t->programa : "-");

    //TESTE 3 -> nome duplicado
    int r2 = task_add("listar", "/bin/ls", args1, 2);
    printf("task_add(listar) duplicado -> %d (esperado -1)\n", r2);

    /* TESTE 4 -> nome inexistente */
    Task *t2 = task_find("naoexiste");
    printf("task_find(naoexiste) -> %s (esperado NULL)\n", t2 ? "achou (ERRO)" : "NULL");

    // TESTE 5 -> job_add + job_find 
    int jid = job_add(1234, "listar");
    printf("job_add -> ID=%d (esperado 1)\n", jid);

    process *p = job_find(jid);
    printf("job_find(%d) -> %s", jid, p ? "achou" : "NULL");
    if (p) {
        printf(", pid=%d, command=%s, estado=%d, saida=%d (esperado saida=-1)\n",
               p->pid, p->command, p->estado, p->saida);
    } else {
        printf("\n");
    }

    // TESTE 6 ->id inexistente 
    process *p2 = job_find(999);
    printf("job_find(999) -> %s (esperado NULL)\n", p2 ? "achou (ERRO)" : "NULL");

    return 0;
}