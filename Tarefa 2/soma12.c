#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


#define N 100000000 // Tamanho do vetor
int A[N];

// Mede o tempo de execução
double get_time() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec / 1000000.0;
}

int main() {

    double start, end;
    int soma, soma1, soma2;

    // 1º Laço: Inicialização do vetor
    start = get_time();
    for (int i = 0; i < N; i++) {
        A[i] = i;
    }
    end = get_time();
    printf("Tempo (inicializacao): %.6f segundos\n", end - start);

    // 2º Laço: Soma acumulativa (dependência entre iterações)
    start = get_time();
    soma = 0;
    for (int i = 0; i < N; i++) {
        soma += A[i];
    }
    end = get_time();
    printf("Tempo (soma dependente): %.6f segundos\n", end - start);
    printf("Soma: %d\n", soma);

    return 0;
}