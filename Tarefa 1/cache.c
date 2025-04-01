#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define TAM 3000
int matrix[TAM][TAM];
int vetor[TAM];
int vetorfinal[TAM];

int main() {
    struct timeval start, end, start2, end2;
    
    printf("Tamanho do vetor: %d. Tamanho da matrix: %dX%d\n", TAM, TAM, TAM);
   
    // Preenchendo a matriz e o vetor com números aleatórios
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            matrix[i][j] = rand() % 100; // Gerar números aleatórios entre 0 e 99
        }
        vetor[i] = rand() % 100; // Gerar números aleatórios para o vetor
        vetorfinal[i] = 0; // Inicializando o vetorfinal
    }

    // Obtenha o tempo de início
    gettimeofday(&start, NULL);
    
    // Multiplicação da matriz pelo vetor (percorrendo as linhas)
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            vetorfinal[i] += matrix[i][j] * vetor[j];
        }
    }

     // Obtenha o tempo de término
     gettimeofday(&end, NULL);

     // Calcule o tempo decorrido
     long seconds = end.tv_sec - start.tv_sec;
     long microseconds = end.tv_usec - start.tv_usec;
     if (microseconds < 0) {
         seconds--;
         microseconds += 1000000;
     }
 
     double elapsed = seconds + microseconds / 1000000.0;
 
     // Exibe o tempo de execução
     printf("Tempo de execucao: %.6f segundos percorrendo as linhas primeiro\n", elapsed);
 

    // Obtenha o tempo de início
    gettimeofday(&start2, NULL);

    // Multiplicação da matriz pelo vetor (percorrendo os vetores)
    for (int j = 0; j < TAM; j++) {
        for (int i = 0; i < TAM; i++) {
            vetorfinal[j] += matrix[i][j] * vetor[i];
        }
    }

    // Obtenha o tempo de término
    gettimeofday(&end2, NULL);

    // Calcule o tempo decorrido
    long seconds2 = end2.tv_sec - start2.tv_sec;
    long microseconds2 = end2.tv_usec - start2.tv_usec;
    if (microseconds2 < 0) {
        seconds2--;
        microseconds2 += 1000000;
    }

    double elapsed2 = seconds2 + microseconds2 / 1000000.0;

    // Exibe o tempo de execução
    printf("Tempo de execucao: %.6f segundos percorrendo as colunas primeiro\n", elapsed2);

    return 0;
}