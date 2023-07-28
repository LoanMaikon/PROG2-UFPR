#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "lista.h"

/* aloca espaco para a cabeca da lista */
cabeca_lista_t *cria_lista()
{
    cabeca_lista_t *lista;

    if (!(lista = malloc(sizeof(cabeca_lista_t))))
        return NULL;

    lista->primeiro = NULL;

    return lista;
}

/* desaloca espaco da lista */
cabeca_lista_t *destroi_lista(cabeca_lista_t *lista)
{
    nodo_lista_t *aux;

    while (lista->primeiro != NULL)
    {
        aux = lista->primeiro;
        lista->primeiro = lista->primeiro->prox;
        free(aux);
    }

    free(lista);

    return NULL;
}

/* copia de dados */
nodo_lista_t *copia_dados(wchar_t caracter, int codigo)
{
    nodo_lista_t *copia;

    if (!(copia = malloc(sizeof(nodo_lista_t))))
        return 0;

    copia->caracter = caracter;
    copia->codigo = codigo;

    return copia;
}

/* adiciona o nodo com seu lugar correto na lista */
void adiciona(cabeca_lista_t *lista, nodo_lista_t *novo, wchar_t caracter)
{
    nodo_lista_t *aux;

    /* caso seja o primeiro nodo da lista */
    if (lista->primeiro == NULL)
    {
        lista->primeiro = novo;
        novo->prox = NULL;
        return;
    }

    aux = lista->primeiro;

    /* caso o nodo deva ser inserido no inicio da lista */
    if (caracter < lista->primeiro->caracter)
    {
        lista->primeiro = novo;
        novo->prox = aux;
        return;
    }

    /* caso o nodo deva ser inserido na ordem da lista */
    while ((aux->prox != NULL) && (caracter >= aux->prox->caracter))
        aux = aux->prox;
    novo->prox = aux->prox;
    aux->prox = novo;
}

/* adiciona um nodo na ordem do caracter ascii */
void adiciona_ordem_lista(cabeca_lista_t *lista, wchar_t caracter, int codigo)
{
    nodo_lista_t *novo;

    /* copia os dados e aloca espaco */
    novo = copia_dados(caracter,codigo);

    /* operacao para verificar como o nodo sera adicionado */
    adiciona(lista,novo,caracter);
}