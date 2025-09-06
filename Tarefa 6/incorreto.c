#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

#define TOTAL_PONTOS 100000000

// Função para medir o tempo de execução
double get_tempo() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
}

int main() {
    long pontos_no_circulo = 0;
    double inicio, fim;

    inicio = get_tempo();
    
    #pragma omp parallel for
    for (int i = 0; i < TOTAL_PONTOS; i++) {
        double x = (double)rand() / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand() / RAND_MAX * 2.0 - 1.0;
        
        if (x * x + y * y <= 1.0) {
            pontos_no_circulo++;
        }
    }

    fim = get_tempo();
    double pi_estimado = 4.0 * (double)pontos_no_circulo / TOTAL_PONTOS;
    
    printf("Tempo de execucao: %f segundos\n", fim - inicio);
    printf("Pontos dentro do circulo: %d\n", pontos_no_circulo);
    printf("Estimativa de PI: %f\n", pi_estimado);

    return 0;
}