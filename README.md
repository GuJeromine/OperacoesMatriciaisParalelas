# Benchmarking de Operações de Matriz com OpenMP

Este é um programa em C++ que utiliza a biblioteca OpenMP para realizar operações aritméticas elemento a elemento em matrizes de grande dimensão (definidas por `N=7000`).

## Objetivo

O objetivo principal do código é **avaliar e comparar o desempenho (speedup)** da computação paralela. O programa mede o tempo de execução das operações da matriz enquanto varia o número de threads, começando de 1 (execução sequencial) até o número máximo de threads disponíveis no sistema (`omp_get_max_threads()`).

## Como Funciona

1.  **Alocação:** Três matrizes (`A`, `B`, `C`) de tamanho `N x N` (`7000x7000`) são alocadas dinamicamente na memória.

2.  **Inicialização:** As matrizes `A` e `B` são preenchidas com valores (`A[i][j] = i + 1` e `B[i][j] = i + 1`) de forma sequencial.

3.  **Loop de Benchmark:** O programa entra em um loop principal que itera de `num_threads = 1` até o máximo de threads que o processador suporta.

4.  **Medição de Tempo:** Dentro do loop, para cada contagem de threads, o tempo de início é registrado usando `omp_get_wtime()`.

5.  **Região Paralela:**
    * O número de threads para a próxima região paralela é definido com `omp_set_num_threads(num_threads)`.
    * Uma região paralela é iniciada com `#pragma omp parallel`. As variáveis de loop `i`, `j` e o `tid` (thread ID) são declaradas como `private`, garantindo que cada thread tenha sua própria cópia.
    * **Distribuição de Trabalho:** A diretiva `#pragma omp for` é aplicada ao loop externo (o que itera sobre as linhas `i`). Isso divide automaticamente as linhas da matriz entre as threads disponíveis. Cada thread processa um "bloco" de linhas.

6.  **Cálculo:** Cada thread executa o loop interno (`j`) para as linhas que lhe foram atribuídas, realizando um conjunto de operações em cada elemento e armazenando o resultado em `C[i][j]`:
    ```cpp
    C[i][j] = A[i][j] + B[i][j]; // Soma
    C[i][j] += A[i][j] * B[i][j]; // Multiplicação e soma
    C[i][j] -= A[i][j] / (B[i][j] + 1); // Divisão e subtração
    ```

7.  **Resultados:** Ao final da região paralela, o tempo de término é registrado. O programa então imprime o tempo total de execução para aquela contagem de threads.

8.  **Limpeza:** Após o término do loop de benchmark (quando todas as contagens de threads foram testadas), toda a memória alocada dinamicamente para as matrizes `A`, `B`, e `C` é liberada usando `delete[]`.
