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

void print_list(Node* head, int idx) {
    printf("Lista %d: ", idx);
    for (; head; head = head->next)
        printf("%d -> ", head->value);
    printf("NULL\n");
}

double tempo_em_segundos(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
}

int main() {
    int N = 20;
    int M;

    printf("Digite o numero de listas: ");
    scanf("%d", &M);

    Node** listas = calloc(M, sizeof(Node*));
    omp_lock_t* locks = malloc(M * sizeof(omp_lock_t));

    for (int i = 0; i < M; i++)
        omp_init_lock(&locks[i]);

    struct timeval inicio, fim;
    gettimeofday(&inicio, NULL);

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < N; i++) {
                int valor = rand() % 1000;
                int escolha = rand() % M;

                #pragma omp task firstprivate(valor, escolha)
                {
                    omp_set_lock(&locks[escolha]);
                    insert(&listas[escolha], valor);
                    omp_unset_lock(&locks[escolha]);
                }
            }
        }
    }

    gettimeofday(&fim, NULL);
    double tempo = tempo_em_segundos(inicio, fim);
    printf("\nTempo de execuçao: %.6f segundos\n", tempo);

    for (int i = 0; i < M; i++) {
        print_list(listas[i], i);
        omp_destroy_lock(&locks[i]);
    }

    free(listas);
    free(locks);
    return 0;
}
