#ifndef LISTA_H
#define LISTA_H

typedef struct nodo_lista
{
    struct nodo_lista *prox;
    wchar_t caracter;
    int codigo;
} nodo_lista_t;

typedef struct cabeca_lista
{
    nodo_lista_t *primeiro;
} cabeca_lista_t;

cabeca_lista_t *cria_lista();

cabeca_lista_t *destroi_lista(cabeca_lista_t *lista);

void adiciona_ordem_lista(cabeca_lista_t *lista, wchar_t caracter, int codigo);

#endif