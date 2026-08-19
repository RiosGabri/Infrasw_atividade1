# Arquitetura:

```
grp/
|
|-- processflow.C
|-- processflow.h
|-- evidencias.log
|-- makefile
|-- readme
```
no final ser compactado para .tar -> grp.tar
```bash
relatório será em .pdf  e mandar tudo para o classroom junto do link p/ classroom
```

```
No terminal, coloco:cd ~/GRP/Infrasw_atividade1 
     -script -a ../evidencias.log
     - gcc -Wall -Wextra -std=c11 processflow.c -o processflow
     - echo "Testeterminal" | ./processflow
     - exit (grava e finaliza o script)
     - Repete o primeiro quando voltar p/ IDE
     - Repete o segundo p/ iniciar o script

```
-Terça 18/08:
   -task -> molde das tarefas = cadastro de tarefa + guardar na memória
        -nome: Apelido dos comandos, programa: executável e argv: argumento
   - control -> painel de controle + registro p/ não "bloquear o painel
        - identificação (id e pid) + Prevenção de processos zumbis + estado de controle para análise se processo ainda está acontecendo