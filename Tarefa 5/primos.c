#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int eh_primo(int num) {
    if (num < 2) return 0;
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

int main() {
    int n;
    printf("Digite o valor de n: ");
    scanf("%d", &n);

    // === SEQUENCIAL ===
    int total_seq = 0;
    clock_t inicio_seq = clock();
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) {
            total_seq++;
        }
    }
    clock_t fim_seq = clock();
    double tempo_seq = (double)(fim_seq - inicio_seq) / CLOCKS_PER_SEC;

    // === PARALELO ===
    int total_par = 0;
    double inicio_par = omp_get_wtime();

    #pragma omp parallel for reduction(+:total_par)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) {
            total_par++;
        }
    }

    double fim_par = omp_get_wtime();
    double tempo_par = fim_par - inicio_par;

    // === RESULTADOS ===
    printf("\n--- RESULTADOS ---\n");
    printf("Sequencial:\n");
    printf("  Primos encontrados: %d\n", total_seq);
    printf("  Tempo: %.4f segundos\n", tempo_seq);

    printf("\nParalelo:\n");
    printf("  Primos encontrados: %d\n", total_par);
    printf("  Tempo: %.4f segundos\n", tempo_par);

    // Verificando consistência
    if (total_seq == total_par) {
        printf("\n✅ Os resultados coincidem.\n");
    } else {
        printf("\n⚠️  Os resultados são diferentes! Verifique a lógica.\n");
    }

    printf("\n🔢 Quantidade total de números primos entre 2 e %d: %d\n", n, total_seq);

    return 0;
}
