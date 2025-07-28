#ifndef ATLETAS_H
#define ATLETAS_H

typedef struct {
    char nome[50];
    char posicao[50];
    int idade;
} Atleta;

typedef struct No {
    Atleta atleta;
    struct No* prox;
} No;

// Funções principais
typedef int bool;
#define true 1
#define false 0

No* carregarAtletas();
void salvarAtletas(No* lista);
void inserirAtleta(No** lista, Atleta novo);
void listarAtletas(No* lista);
void ordenarPorNome(No** lista);
void buscarBinariaNome(No* lista, const char* nome);
void editarAtleta(No* lista, const char* nomeBusca);
void deletarAtleta(No** lista, const char* nomeBusca);
void liberarLista(No* lista);

#endif
