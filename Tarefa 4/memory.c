#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>

#define N 10000000 // Tamanho do vetor
#define NUM_ITER 100000000 // Número de iterações para carga computacional

// Função para obter o tempo atual em segundos
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

void memory_bound(int num_threads, double *a, double *b, double *c, FILE *file) {
    double start = get_time();
    
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }
    
    double end = get_time();
    double time_taken = end - start;
    printf("Memory-bound (%d threads): %f seconds\n", num_threads, time_taken);
    fprintf(file, "%d,memory,%f\n", num_threads, time_taken);
}

void compute_bound(int num_threads, double *results, FILE *file) {
    double start = get_time();
    
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < NUM_ITER; i++) {
        results[i] = sqrt(i) * exp(i * 0.000001);
    }
    
    double end = get_time();
    double time_taken = end - start;
    printf("Compute-bound (%d threads): %f seconds\n", num_threads, time_taken);
    fprintf(file, "%d,compute,%f\n", num_threads, time_taken);
}

int main() {
    static double a[N], b[N], c[N]; // Vetores estáticos
    static double results[NUM_ITER]; // Vetor para armazenar resultados computacionais
    
    FILE *file = fopen("performance.csv", "w");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return 1;
    }
    fprintf(file, "threads,type,time\n");
    
    for (int i = 0; i < N; i++) {
        a[i] = i * 1.0;
        b[i] = (N - i) * 1.0;
    }
    
    for (int num_threads = 1; num_threads <= 16; num_threads++) {
        memory_bound(num_threads, a, b, c, file);
        compute_bound(num_threads, results, file);
    }
    
    fclose(file);
    printf("Resultados salvos em performance.csv\n");
    return 0;
}
