#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "atletas.h"

void inserirAtleta(No** lista, Atleta novo) {
    No* novoNo = malloc(sizeof(No));
    novoNo->atleta = novo;
    novoNo->prox = *lista;
    *lista = novoNo;
}

void listarAtletas(No* lista) {
    while (lista) {
        printf("Nome: %s | Posicao: %s | Idade: %d\n",
               lista->atleta.nome,
               lista->atleta.posicao,
               lista->atleta.idade);
        lista = lista->prox;
    }
}

void salvarAtletas(No* lista) {
    FILE* file = fopen("atletas.txt", "w");
    if (!file) return;

    while (lista) {
        fprintf(file, "%s;%s;%d\n",
                lista->atleta.nome,
                lista->atleta.posicao,
                lista->atleta.idade);
        lista = lista->prox;
    }
    fclose(file);
}

No* carregarAtletas() {
    FILE* file = fopen("atletas.txt", "r");
    if (!file) return NULL;

    No* lista = NULL;
    Atleta temp;
    while (fscanf(file, " %49[^;];%49[^;];%d\n",
                  temp.nome, temp.posicao, &temp.idade) == 3) {
        inserirAtleta(&lista, temp);
    }

    fclose(file);
    return lista;
}

No* merge(No* a, No* b) {
    if (!a) return b;
    if (!b) return a;

    No* resultado = NULL;
    if (strcmp(a->atleta.nome, b->atleta.nome) <= 0) {
        resultado = a;
        resultado->prox = merge(a->prox, b);
    } else {
        resultado = b;
        resultado->prox = merge(a, b->prox);
    }
    return resultado;
}

void dividir(No* origem, No** frente, No** tras) {
    No* rapido;
    No* devagar;
    devagar = origem;
    rapido = origem->prox;

    while (rapido) {
        rapido = rapido->prox;
        if (rapido) {
            devagar = devagar->prox;
            rapido = rapido->prox;
        }
    }

    *frente = origem;
    *tras = devagar->prox;
    devagar->prox = NULL;
}

void mergeSort(No** cabecaRef) {
    No* cabeca = *cabecaRef;
    if (!cabeca || !cabeca->prox)
        return;

    No* a;
    No* b;
    dividir(cabeca, &a, &b);
    mergeSort(&a);
    mergeSort(&b);
    *cabecaRef = merge(a, b);
}

void ordenarPorNome(No** lista) {
    mergeSort(lista);
}

void buscarBinariaNome(No* lista, const char* nomeBusca) {
    int tamanho = 0;
    No* temp = lista;
    while (temp) {
        tamanho++;
        temp = temp->prox;
    }

    Atleta* vetor = malloc(tamanho * sizeof(Atleta));
    temp = lista;
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = temp->atleta;
        temp = temp->prox;
    }

    int esquerda = 0, direita = tamanho - 1;
    while (esquerda <= direita) {
        int meio = (esquerda + direita) / 2;
        int cmp = strcmp(vetor[meio].nome, nomeBusca);
        if (cmp == 0) {
            printf("Atleta encontrado: %s | Posicao: %s | Idade: %d\n",
                   vetor[meio].nome, vetor[meio].posicao, vetor[meio].idade);
            free(vetor);
            return;
        } else if (cmp < 0) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }
    printf("Atleta nao encontrado.\n");
    free(vetor);
}

void editarAtleta(No* lista, const char* nomeBusca) {
    while (lista) {
        if (strcmp(lista->atleta.nome, nomeBusca) == 0) {
            printf("Novo nome: ");
            fgets(lista->atleta.nome, 50, stdin);
            lista->atleta.nome[strcspn(lista->atleta.nome, "\n")] = '\0';

            printf("Nova posicao: ");
            fgets(lista->atleta.posicao, 50, stdin);
            lista->atleta.posicao[strcspn(lista->atleta.posicao, "\n")] = '\0';

            printf("Nova idade: ");
            scanf("%d", &lista->atleta.idade);
            getchar();
            printf("Atleta editado com sucesso!\n");
            return;
        }
        lista = lista->prox;
    }
    printf("Atleta nao encontrado.\n");
}

void deletarAtleta(No** lista, const char* nomeBusca) {
    No *atual = *lista, *anterior = NULL;
    while (atual) {
        if (strcmp(atual->atleta.nome, nomeBusca) == 0) {
            if (anterior == NULL) {
                *lista = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            printf("Atleta excluido com sucesso.\n");
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
    printf("Atleta nao encontrado.\n");
}

void liberarLista(No* lista) {
    No* temp;
    while (lista) {
        temp = lista;
        lista = lista->prox;
        free(temp);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "atletas.h"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int emailValido(const char *email) {
    return strlen(email) < 80 && strchr(email, '@') != NULL;
}

int login() {
    char usuario[20], senha[20];
    while (1) {
        printf("\n      Login      \n");
        printf("Usuario: ");
        fgets(usuario, sizeof(usuario), stdin);
        usuario[strcspn(usuario, "\n")] = '\0';

        printf("Senha: ");
        fgets(senha, sizeof(senha), stdin);
        senha[strcspn(senha, "\n")] = '\0';

        if (strcmp(usuario, "admin") == 0 && strcmp(senha, "admin") == 0) {
            printf("Login bem-sucedido!\n");
            return 1;
        } else {
            printf("Usuario ou senha incorretos.\n");
        }
    }
}


int main() {
    if (!login()) return 0;

    char email[80];
    do {
        printf("Informe seu e-mail: ");
        fgets(email, 80, stdin);
        email[strcspn(email, "\n")] = '\0';
        if (!emailValido(email))
            printf("E-mail invalido.\n");
    } while (!emailValido(email));

    printf("\nBem-vindo ao Sistema de Cadastro de Atletas de Volei!\n");

    No* lista = carregarAtletas();

    int opcao;
    do {
        printf("\n1 - Cadastrar novo atleta\n");
        printf("2 - Listar atletas\n");
        printf("3 - Ordenar por nome\n");
        printf("4 - Buscar por nome (binaria)\n");
        printf("5 - Editar atleta\n");
        printf("6 - Deletar atleta\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            Atleta novo;
            printf("Nome: "); fgets(novo.nome, 50, stdin);
            novo.nome[strcspn(novo.nome, "\n")] = '\0';
            printf("Posicao: "); fgets(novo.posicao, 50, stdin);
            novo.posicao[strcspn(novo.posicao, "\n")] = '\0';
            printf("Idade: "); scanf("%d", &novo.idade);
            limparBuffer();
            inserirAtleta(&lista, novo);
            salvarAtletas(lista);
        } else if (opcao == 2) {
            listarAtletas(lista);
        } else if (opcao == 3) {
            ordenarPorNome(&lista);
            salvarAtletas(lista);
            printf("Ordenado com sucesso!\n");
        } else if (opcao == 4) {
            char nome[50];
            printf("Nome para buscar: "); fgets(nome, 50, stdin);
            nome[strcspn(nome, "\n")] = '\0';
            ordenarPorNome(&lista);
            buscarBinariaNome(lista, nome);
        } else if (opcao == 5) {
            char nome[50];
            printf("Nome para editar: "); fgets(nome, 50, stdin);
            nome[strcspn(nome, "\n")] = '\0';
            editarAtleta(lista, nome);
            salvarAtletas(lista);
        } else if (opcao == 6) {
            char nome[50];
            printf("Nome para excluir: "); fgets(nome, 50, stdin);
            nome[strcspn(nome, "\n")] = '\0';
            deletarAtleta(&lista, nome);
            salvarAtletas(lista);
        }
    } while (opcao != 0);

    liberarLista(lista);
    return 0;
}
