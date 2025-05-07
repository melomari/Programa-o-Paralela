#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

typedef struct Node {
    int value;
    struct Node* next;
} Node;

void insert(Node** head, int value) {
    Node* new_node = malloc(sizeof(Node));
    new_node->value = value;
    new_node->next = *head;
    *head = new_node;
}

void print_list(Node* head, const char* name) {
    printf("Lista %s: ", name);
    for (; head; head = head->next)
        printf("%d -> ", head->value);
    printf("NULL\n");
}

double tempo_em_segundos(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
}

int main() {
    int N = 20;
    Node* lista1 = NULL;
    Node* lista2 = NULL;

    struct timeval inicio, fim;
    gettimeofday(&inicio, NULL);

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < N; i++) {
                int valor = rand() % 1000;
                int escolha = rand() % 2;

                #pragma omp task firstprivate(valor, escolha)
                {
                    if (escolha == 0) {
                        #pragma omp critical(lista1)
                        insert(&lista1, valor);
                    } else {
                        #pragma omp critical(lista2)
                        insert(&lista2, valor);
                    }
                }
            }
        }
    }

    gettimeofday(&fim, NULL);
    double tempo = tempo_em_segundos(inicio, fim);
    printf("\nTempo de execuçao: %.6f segundos\n", tempo);

    print_list(lista1, "1");
    print_list(lista2, "2");

    return 0;
}
