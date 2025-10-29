#include <iostream>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

// Define o tamanho de N
#define N 7000

int main(int argc, char** argv)
{
    int i, j;
    int **A, **B, **C;
    double start_time, end_time;
    int tid;

    // Aloca a memória para as matrizes A, B e C
    A = new int*[N];
    B = new int*[N];
    C = new int*[N];
    for (i = 0; i < N; i++)
    {
        A[i] = new int[N];
        B[i] = new int[N];
        C[i] = new int[N];
    }

    //Sequencial
    // Inicialização das matrizes A e B
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            A[i][j] = i + 1;
            B[i][j] = i + 1;
        }
    }

    // Passa o número máximo de threads para variável max_threads
    int max_threads = omp_get_max_threads();

    // Testando diferentes números de threads e medindo o tempo de cada
    for (int num_threads = 1; num_threads <= max_threads; num_threads++)
    {
        // Começa medir o tempo
        start_time = omp_get_wtime();

        omp_set_num_threads(num_threads);

        // Inicio da area paralela
        #pragma omp parallel private(tid, i, j) // as variáveis tid, i e j como privadas, fazendo com que cada thread tem sua própria cópia dessas variáveis, e evitar que sejam compartilhadas as mesma variáveis entre as threads
        {
            tid = omp_get_thread_num();
            printf("Hello World from thread = %d\n", tid); // aparentemente não funciona com cout
            #pragma omp for
            for (i = 0; i < N; i++)
            {
                for (j = 0; j < N; j++)
                {
                    C[i][j] = A[i][j] + B[i][j]; // soma de elementos
                    C[i][j] += A[i][j] * B[i][j]; // multiplicacao dos elementos das matrizes A e B e soma do resultado com o valor existente na matriz C na mesma posição
                    C[i][j] -= A[i][j] / (B[i][j] + 1); // divisao do elemento de A pelo elemento de B mais 1, e subtração desse resultado com o valor atual na matriz C na mesma posição
                }
            }
        }

        // Para de medir o tempo
        end_time = omp_get_wtime();

        cout << "Tempo de execucao com " << num_threads << " threads: " << end_time - start_time << " segundos" << endl;
    }

    // Libera a memória alocada
    for (i = 0; i < N; i++)
    {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    return (0);
}
