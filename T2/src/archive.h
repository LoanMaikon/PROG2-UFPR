#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lista.h"

int existe(FILE *archive, char *membro);

long tamanho_archive(FILE *archive);

int vazio(FILE *archive);

void inserir_conteudo(FILE *archive, FILE *arq);

void trunca_diretorio(FILE *archive);

void iniciar_archive(FILE *archive);

int atualiza_pos_dir(cabeca_lista_t *lista);

void atualiza_inicio(FILE *archive, cabeca_lista_t *lista);

void remove_membro_conteudo(cabeca_lista_t *lista, FILE *archive, char *membro);

void cria_membro(cabeca_lista_t *lista, FILE *archive, char *membro);

#endif