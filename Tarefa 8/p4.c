#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define NUM_POINTS 1000000000

int main() {
    int num_threads;
    struct timeval t_start, t_end;

    gettimeofday(&t_start, NULL);

    #pragma omp parallel
    {
        #pragma omp single
        num_threads = omp_get_num_threads();
    }

    int *counts = calloc(num_threads, sizeof(int));

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        unsigned int seed = time(NULL) ^ tid;

        #pragma omp for
        for (int i = 0; i < NUM_POINTS; i++) {
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;
            if (x*x + y*y <= 1.0)
                counts[tid]++;
        }
    }

    int total = 0;
    for (int i = 0; i < num_threads; i++)
        total += counts[i];

    gettimeofday(&t_end, NULL);
    double elapsed = (t_end.tv_sec - t_start.tv_sec)
                   + (t_end.tv_usec - t_start.tv_usec)/1e6;
    double pi = 4.0 * total / NUM_POINTS;

    /* Contagem de casas decimais corretas */
    double real_pi = acos(-1.0);
    char buf_est[32], buf_real[32];
    sprintf(buf_est,  "%.15f", pi);
    sprintf(buf_real, "%.15f", real_pi);
    char *pe = strchr(buf_est,  '.');
    char *pr = strchr(buf_real, '.');
    int correct_dec = 0;
    if (pe && pr) {
        pe++; pr++;
        while (*pe && *pr && *pe == *pr) {
            correct_dec++;
            pe++; pr++;
        }
    }

    printf("Pi (vetor compartilhado, rand)          = %.10f\n", pi);
    printf("Time                   = %.5f seconds\n", elapsed);
    printf("Decimal places correct = %d\n", correct_dec);
        

    free(counts);
    return 0;
}
