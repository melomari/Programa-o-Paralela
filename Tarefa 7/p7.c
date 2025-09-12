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
    append_node(&head, "arquivo6.txt");
    append_node(&head, "arquivo7.txt");
    append_node(&head, "arquivo8.txt");
    append_node(&head, "arquivo9.txt");
    append_node(&head, "arquivo10.txt");
    append_node(&head, "arquivo11.txt");
    append_node(&head, "arquivo12.txt");
    append_node(&head, "arquivo13.txt");
    append_node(&head, "arquivo14.txt");
    append_node(&head, "arquivo15.txt");

    printf("### Execucao com SINGLE + TASK ###\n");

    #pragma omp parallel
    {
        #pragma omp single 
        {
            int tid = omp_get_thread_num();
            printf("[SINGLE] Thread %d criou as tarefas\n", tid);
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
            #pragma omp taskwait
            {
                printf("[SINGLE] Todas as tarefas foram concluidas.\n");
            }
        }

    }

    printf("\n### Execucao com MASTER E SEM SINGLE + TASK ###\n");

    #pragma omp parallel
    {
        #pragma omp master
        {
            int tid = omp_get_thread_num();
            printf("[MASTER] Thread %d criou as tarefas\n", tid);
            Node* current = head;
            while (current != NULL) {
                Node* node = current;
                #pragma omp task firstprivate(node)
                {
                    int tid = omp_get_thread_num();
                    printf("[MASTER] Thread %d processando %s\n", tid, node->filename);
                }

                current = current->next;
            }
            #pragma omp taskwait
            {
                printf("[MASTER] Todas as tarefas foram concluidas.\n");
            }
        }
        #pragma omp barrier
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
