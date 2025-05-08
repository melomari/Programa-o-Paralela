#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Estrutura do nó da lista
typedef struct Node {
    char filename[50];
    struct Node* next;
} Node;

// Função para criar um novo nó
Node* create_node(const char* name) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    snprintf(new_node->filename, sizeof(new_node->filename), "%s", name);
    new_node->next = NULL;
    return new_node;
}

// Função para adicionar um nó ao final da lista
void append_node(Node** head, const char* name) {
    Node* new_node = create_node(name);
    if (*head == NULL) {
        *head = new_node;
    } else {
        Node* temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_node;
    }
}

int main() {
    Node* head = NULL;

    // Criar lista com arquivos fictícios
    append_node(&head, "arquivo1.txt");
    append_node(&head, "arquivo2.txt");
    append_node(&head, "arquivo3.txt");
    append_node(&head, "arquivo4.txt");
    append_node(&head, "arquivo5.txt");

    printf("### Execução com SINGLE + TASK ###\n");

    #pragma omp parallel
    {
        #pragma omp single
        {
            Node* current = head;
            while (current != NULL) {
                Node* node = current;
                #pragma omp task firstprivate(node)
                {
                    int tid = omp_get_thread_num();
                    printf("[SINGLE] Thread %d processando %s\n", tid, node->filename);
                }

                current = current->next;
            }
        }

        #pragma omp master
        {
            printf("[SINGLE] Todas as tarefas foram concluidas.\n");
        }
    }

    printf("\n### Execução com SEM MASTER E SEM SINGLE + TASK ###\n");

    #pragma omp parallel
    {
        // #pragma omp master
        // {
            Node* current = head;
            while (current != NULL) {
                Node* node = current;
                #pragma omp task firstprivate(node)
                {
                    int tid = omp_get_thread_num();
                    printf("[SEM MASTER E SEM SINGLE] Thread %d processando %s\n", tid, node->filename);
                }

                current = current->next;
            }
        // }

        // #pragma omp barrier

        #pragma omp master
        {
            printf("[MASTER] Todas as tarefas foram concluidas.\n");
        }
    }

    // Liberar memória
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
