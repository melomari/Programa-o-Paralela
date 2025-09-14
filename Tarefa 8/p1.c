#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

#define NUM_POINTS      1000000000
#define MY_RAND_MAX     2147483647.0

unsigned int my_rand_r(unsigned int *seed) {
    *seed = (*seed * 1103515245 + 12345) & 0x7fffffff;
    return *seed;
}

int main() {
    int inside_circle = 0;
    struct timeval t_start, t_end;
    gettimeofday(&t_start, NULL);

    #pragma omp parallel 
    {
        int local_count = 0;
        unsigned int seed = time(NULL) ^ omp_get_thread_num();

        #pragma omp for 
        for (int i = 0; i < NUM_POINTS; i++) {
            double x = (double)my_rand_r(&seed) / MY_RAND_MAX;
            double y = (double)my_rand_r(&seed) / MY_RAND_MAX;
            if (x*x + y*y <= 1.0)
                local_count++;
        }

        #pragma omp critical
        inside_circle += local_count;
    }

    gettimeofday(&t_end, NULL);
    double elapsed = (t_end.tv_sec - t_start.tv_sec)
                   + (t_end.tv_usec - t_start.tv_usec) / 1e6;

    double pi_est  = 4.0 * inside_circle / NUM_POINTS;
    double pi_real = acos(-1.0);
    double error   = fabs(pi_est - pi_real);

    int correct = 0;
    double tol = 0.5;
    while (correct < 15 && error < tol) {
        correct++;
        tol *= 0.1;
    }

    printf("Pi (critical)          = %.10f\n", pi_est);
    printf("Time                   = %.5f seconds\n", elapsed);
    printf("Decimal places correct = %d\n", correct);

    return 0;
}
