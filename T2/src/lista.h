#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

#ifndef LISTA_H
#define LISTA_H

#define TAM_NOME 1024
#define TAM_LOCALIZACAO 1024

typedef struct nodo_lista
{
    struct nodo_lista *prox;
    char nome[TAM_NOME];
    uid_t uid;
    mode_t permissoes;
    off_t tamanho;
    time_t data_mod;
    int ordem;
    char local[TAM_LOCALIZACAO];
    gid_t gid;
} nodo_lista_t;

typedef struct cabeca_lista
{
    nodo_lista_t *primeiro;
} cabeca_lista_t;

char *caminho_relativo(char *caminho, char *membro);

cabeca_lista_t *cria_lista();

cabeca_lista_t *destroi_lista(cabeca_lista_t *lista);

nodo_lista_t *cria_nodo(cabeca_lista_t *lista, char *membro, FILE *archive);

void adiciona_membro_lista(FILE *archive, cabeca_lista_t *lista, char *membro);

void escreve_lista(cabeca_lista_t *lista, FILE *archive);

void adiciona_nodo(cabeca_lista_t *lista, char nome[TAM_NOME], uid_t uid, mode_t permissoes,
off_t tamanho, time_t data_mod, int ordem, char local[TAM_LOCALIZACAO], gid_t gid);

void inicia_lista(cabeca_lista_t *lista, FILE *archive);

long tamanho_conteudo(cabeca_lista_t *lista);

void remove_membro_lista(cabeca_lista_t *lista, char *membro);

int compara_datamod(FILE *archive, char *membro);

void mover_membro(cabeca_lista_t *lista, char *membro, char *target);

void imprime_lista(cabeca_lista_t *lista);

void cria_diretórios(char *membro);

#endif