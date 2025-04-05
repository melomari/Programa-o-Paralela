#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

// Função para calcular Pi usando a série de Leibniz
double calcular_pi(int iteracoes) {
    double pi = 0.0;
    for (int i = 0; i < iteracoes; i++) {
        double termo = (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);
        pi += termo;
    }
    return pi * 4;
}

// Função para contar quantos dígitos coincidem entre pi_real e pi_aprox
int contar_digitos_corretos(double pi_aprox, double pi_real) {
    char str_aprox[50], str_real[50];
    snprintf(str_aprox, sizeof(str_aprox), "%.15f", pi_aprox);
    snprintf(str_real, sizeof(str_real), "%.15f", pi_real);
    
    int count = 0;
    for (int i = 0; str_real[i] != '\0' && str_aprox[i] != '\0'; i++) {
        if (str_real[i] == str_aprox[i]) {
            count++;
        } else {
            break;
        }
    }
    return count - 2; // Subtrai "3." do início da contagem
}

int main() {
    FILE *fp = fopen("dados_pi.csv", "w"); // Arquivo CSV para armazenar os dados
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo para escrita!\n");
        return 1;
    }
    
    // Escrevendo cabeçalho do CSV
    fprintf(fp, "Iteracoes,Aproximacao_PI,PI_Real,Erro_Absoluto,Tempo_ms,Precisao,Digitos_Corretos\n");
    
    printf("Iteracoes | Aproximacao_PI | PI_Real | Erro_Absoluto | Tempo_ms | Precisao | Digitos_Corretos\n");
    printf("--------------------------------------------------------------------------------------------\n");
    
    for (int iteracoes = 100; iteracoes <= 1000000; iteracoes += 100) {
        struct timeval inicio, fim;
        gettimeofday(&inicio, NULL);
        
        double pi_aprox = calcular_pi(iteracoes);
        double pi_real = M_PI; // Obtendo o valor de PI da biblioteca math.h
        
        gettimeofday(&fim, NULL);
        double tempo_execucao = (fim.tv_sec - inicio.tv_sec) * 1000.0;
        tempo_execucao += (fim.tv_usec - inicio.tv_usec) / 1000.0;
        
        double erro = fabs(pi_real - pi_aprox);
        double precisao = (1 - (erro / pi_real)) * 100;
        int digitos_corretos = contar_digitos_corretos(pi_aprox, pi_real);
        
        // Escrevendo dados no CSV
        fprintf(fp, "%d,%.10f,%.15f,%.10f,%.3f,%.2f,%d\n", iteracoes, pi_aprox, pi_real, erro, tempo_execucao, precisao, digitos_corretos);
        
        // Printando no terminal
        printf("%9d | %.10f | %.15f | %.10f | %.3f | %.2f | %d\n", iteracoes, pi_aprox, pi_real, erro, tempo_execucao, precisao, digitos_corretos);
    }
    
    fclose(fp);
    printf("Os dados foram salvos em 'dados_pi.csv'.\n");
    return 0;
}
