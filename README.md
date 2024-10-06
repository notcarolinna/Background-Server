# Escalonamento Backgroud Server com Razão Monotônica
O algoritmo implementa um simulador de escalonamento para um Background Server, que gerencia tarefas periódicas e aperiódicas. As tarefas periódicas têm prioridade sobre as aperiódicas e são escalonadas com base em seus períodos. As tarefas aperiódicas são organizadas em uma fila FIFO.

## Entrada
A primeira linha de cada conjunto de teste contém três números inteiros:
  - Tempo de simulação (T)
  - Quantidade de tarefas periódicas (TP)
  - Quantidade de tarefas aperiódicas (TA)
Em seguida, são fornecidas as descrições das tarefas. Primeiro, as tarefas periódicas, com seus tempos de computação, períodos e deadlines. Depois, as tarefas aperiódicas, com o tempo de chegada e o tempo de computação. A entrada termina com T=0, TP=0 e TA=0, indicando o fim do conjunto de teste.

## Saída
Para cada conjunto de testes, a simulação gera e exibe as seguintes informações:
  - Simplificação do diagrama de Gantt
  - Preempções e trocas de contexto
  - Tempo total de execução e espera para cada tarefa