# Relatório do Experimento E1: Comparação de Multiplicação de Matrizes (Sequencial vs Paralelo)

## Resultados
A tabela a seguir detalha os tempos de execução obtidos para cada tamanho de matriz e abordagem:

| Tamanho da Matriz | Sequencial (ms) | Threads (ms) | Processos (ms) |
|-------------------|-----------------|--------------|----------------|
| 100x100           | 0               | 0            | 1              |
| 200x200           | 5               | 1            | 2              |
| 400x400           | 59              | 15           | 17             |
| 800x800           | 701             | 208          | 200            |
| 1600x1600          | 5735            | 1457         | 1532           |


## Análise
Observa-se que, para matrizes pequenas (100x100), os tempos de execução são muito baixos e a diferença entre as abordagens é mínima. Conforme o tamanho da matriz aumenta, a abordagem sequencial demonstra um crescimento exponencial no tempo de execução. As abordagens paralelas (threads e processos) apresentam tempos significativamente menores, resultando em um speedup considerável. O speedup para threads e processos se aproxima do ideal (4x) para matrizes maiores, indicando a eficácia da paralelização para problemas computacionalmente intensivos.


# Relatório do Experimento E2: Impacto do Número de Threads/Processos (P)

## Resultados
A tabela a seguir detalha os tempos de execução e o speedup obtidos para cada valor de P:

| P  | Threads (ms) | Processos (ms) | Speedup Threads | Speedup Processos |
|----|--------------|----------------|-----------------|-------------------|
| 1  | 91606        | 86621          | 0.995           | 1.052             |
| 1  | 93099        | 87776          | 0.967           | 1.025             |
| 2  | 43514        | 43486          | 2.095           | 2.096             |
| 2  | 44072        | 52504          | 2.043           | 1.715             |
| 4  | 32310        | 40419          | 2.821           | 2.255             |
| 4  | 40361        | 47704          | 2.231           | 1.887             |
| 6  | 35794        | 42910          | 2.546           | 2.124             |
| 6  | 43218        | 45898          | 2.083           | 1.961             |
| 8  | 39410        | 45472          | 2.313           | 2.004             |
| 8  | 45571        | 42222          | 1.976           | 2.132             |
| 12 | 32541        | 30672          | 2.801           | 2.972             |
| 12 | 31371        | 33715          | 2.870           | 2.670             |
| 16 | 25333        | 30035          | 3.598           | 3.035             |
| 16 | 21717        | 22220          | 4.146           | 4.052             |


## Análise
Os resultados do Experimento E2 demonstram que o aumento do número de threads/processos (P) geralmente leva a uma redução no tempo de execução e a um aumento no speedup, até certo ponto. Para este experimento, o melhor speedup foi observado com P=16, tanto para threads (4.146x) quanto para processos (4.052x). É importante notar que o speedup ideal linear (P vezes) nem sempre é alcançado devido a overheads de comunicação e sincronização entre os processos/threads. A eficiência (speedup/P) tende a diminuir à medida que P aumenta, indicando que o ganho de desempenho por unidade de recurso adicional se torna menor.

