#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "decodificacao.h"

wchar_t busca_codigo_lista(cabeca_lista_t *lista, int codigo)
{
    nodo_lista_t *aux;

    if (codigo == -1)
        return ' ';
    
    if (codigo == -2)
        return '?';
    
    if (codigo == -3)
        return '\n';

    aux = lista->primeiro;

    while (aux != NULL && aux->codigo != codigo)
        aux = aux->prox;

    /* nao achou o caracter na lista */
    if (aux == NULL)
        return 0;

    return aux->caracter;
}

void imprime_decodificacao(cabeca_lista_t *lista, FILE *msg_codificada, FILE *msg_decodificada)
{
    wchar_t caracter;
    int codigo;

    while (fscanf(msg_codificada,"%d",&codigo) != EOF)
    {
        /* procura o caracter pelo codigo */
        caracter = busca_codigo_lista(lista,codigo);

        /* escreve no arquvio o caracter encontrado */
        fprintf(msg_decodificada,"%lc",caracter);
    }
}