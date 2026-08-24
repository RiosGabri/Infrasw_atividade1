# ProcessFlow - Orquestrador de Tarefas

## 1. Contexto
O **ProcessFlow** é um orquestrador de tarefas desenvolvido em C. Ele permite o cadastro de tarefas e o gerenciamento de sua execução através da criação de processos filhos. O sistema suporta execuções sequenciais, paralelas e encadeadas (pipe), além de permitir a execução de processos em segundo plano (background), redirecionamento de entrada/saída (I/O) e navegação de diretórios. O orquestrador pode ser utilizado de forma interativa via terminal ou através de scripts de fluxo (`.pf`).

## 2. Como Compilar e Executar

### Compilação
Para compilar o projeto manualmente utilizando o GCC, execute o comando abaixo na raiz do projeto:
```bash
make
make clean (para limpar a pasta após uso)
```

### Execução
**Modo Interativo:** Abre um prompt (`processflow>`) para a digitação de comandos em tempo real.
```bash
./processflow
```

**Modo Workflow:** Executa uma sequência de comandos a partir de um arquivo de script (`.pf`).
```bash
./processflow script.pf
```

## 3. Exemplos de Uso (Comandos)

Abaixo estão exemplos simples de como utilizar cada funcionalidade do sistema:

* **Cadastro de Tarefa (`task`)**
  Cadastra uma tarefa no sistema associando um nome a um comando e seus argumentos.
  `task t_listar /bin/ls -l`

* **Execução Sequencial (`run sequential`)**
  Executa múltiplas tarefas em ordem, aguardando a anterior terminar para iniciar a próxima.
  `run sequential t_listar t_outra_tarefa`

* **Execução Paralela (`run parallel`)**
  Inicia múltiplas tarefas simultaneamente.
  `run parallel t_tarefa1 t_tarefa2`

* **Execução com Pipe (`run pipe`)**
  Conecta a saída padrão de uma tarefa com a entrada padrão da próxima.
  `task t_echo /bin/echo "ola mundo"`
  `task t_wc /usr/bin/wc -w`
  `run pipe t_echo t_wc`

* **Processos em Background (`start`, `jobs`, `wait`)**
  Inicia uma tarefa em segundo plano, lista os processos rodando e aguarda sua conclusão.
  `start t_listar`
  `jobs`
  `wait 1` *(Onde 1 é o ID retornado pelo jobs)*

* **Redirecionamento de Arquivos (`output`, `append`, `input`)**
  Redireciona a entrada ou saída das tarefas antes da execução.
  `output t_listar /tmp/saida.txt` *(Sobrescreve o arquivo com a saída)*
  `append t_listar /tmp/saida.txt` *(Adiciona a saída ao final do arquivo)*
  `input t_wc /tmp/saida.txt` *(Lê a entrada a partir do arquivo)*

* **Mudança de Diretório (`workdir`)**
  Altera o diretório de trabalho atual do orquestrador.
  `workdir /tmp`

* **Sair (`exit`)**
  Encerra o orquestrador.
  `exit`