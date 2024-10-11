# Escalonamento Backgroud Server com Razão Monotônica
O algoritmo implementa um simulador de escalonamento para um Background Server, que gerencia tarefas periódicas e aperiódicas. As tarefas periódicas têm prioridade sobre as aperiódicas e são escalonadas com base em seus períodos. As tarefas aperiódicas são organizadas em uma fila FIFO.

## Estrutura Básica
- **`PeriodicTask`**: Representa uma tarefa periódica, com informações sobre tempo de computação, período, deadlines, e tempos de execução e espera.
- **`AperiodicTask`**: Representa uma tarefa aperiódica, com dados sobre o tempo de chegada, tempo de computação e espera.
- **`Cpu`**: Simula o comportamento do processador, responsável por executar as tarefas, verificar deadlines, e controlar preempções e trocas de contexto.

### Funções Importantes
- **`Cpu::load`**: Carrega uma nova tarefa para ser executada, lidando com preempções e trocas de contexto.
- **`Cpu::run`**: Executa uma unidade de tempo da tarefa atual, verificando deadlines e atualizando o grid de execução.
- **`taskInput`**: Lê e organiza as entradas das tarefas periódicas e aperiódicas para a simulação.
- **`Cpu::getGrid`**: Retorna o grid de execução gerado após a simulação.

## Funcionamento
- O código recebe como entrada uma lista de tarefas periódicas e aperiódicas, onde cada tarefa tem seu tempo de execução, período, e deadline definidos.
- A CPU executa as tarefas respeitando seus períodos e deadlines, atualizando o tempo de execução e verificando se as deadlines são cumpridas. Se uma tarefa perder o deadline, o símbolo correspondente é convertido para minúscula no grid de execução.
- Tarefas aperiódicas são executadas assim que chegam (tempo de chegada), respeitando a ordem de chegada.
- O grid de execução (linha do tempo) exibe a execução das tarefas, com "." representando o processador ocioso (idle).

## Entrada
- **Primeira Linha:** O primeiro número indica a duração total da simulação. Os números subsequentes representam a quantidade de tarefas periódicas e aperiódicas, respectivamente.
- **Segunda Linha** Correspondem aos valores do tempo de computação, período e deadline das tarefas servidoras, que neste caso não é utilizada.
  
- **Bloco de Tarefas Periódicas:** Cada linha subsequente neste bloco descreve uma tarefa periódica:
  - **Colunas:** 
    - O primeiro número é o tempo de computação.
    - O segundo número é o período da tarefa.
    - O terceiro número é o deadline da tarefa.
    
- **Bloco de Tarefas Aperiódicas:** Similar ao bloco anterior, as linhas representam tarefas aperiódicas:
  - **Colunas:** 
    - O primeiro número é o tempo de chegada da tarefa.
    - O segundo número é o tempo de computação.

## Saída
- Para cada simulação, o código gera:
  - Um grid de execução indicando quando e quais tarefas foram executadas.
  - O número de preempções e trocas de contexto.
  - Tempos de execução (`ex`) e de espera (`wt`) para cada tarefa.
 
## Execução

- Para executar o algoritmo, é necessário ter um ambiente Linux com todas as dependências para C++ instaladas. Para instalar as dependências, você pode usar o seguinte comando:

  ```bash
  sudo apt-get install build-essential
  ```

- Após instalar as dependências, vá até o diretório do projeto e execute o comando:

  ```bash
  make run
  ```
