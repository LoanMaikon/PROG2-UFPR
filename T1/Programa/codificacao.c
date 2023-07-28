#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include "codificacao.h"

/* gera numero aleatorio entre min e max */
int aleatorio (int min, int max)
{
    if (min == max)
        return 1;
    return min + (rand() % (max - min + 1));
}

int busca_caracter_lista(cabeca_lista_t *lista, wchar_t c)
{
    nodo_lista_t *aux1, *aux2;
    int i;
    int aleat;

    if (c == ' ')
        return -1;
    
    if (c == '\n')
        return -3;

    if (lista->primeiro == NULL)
        return -2;
        
    /* procura na lista */
    aux1 = lista->primeiro;
    while (aux1 != NULL && aux1->caracter != c)
        aux1 = aux1->prox;
    
    /* nao achou o caracter na lista */
    if (aux1 == NULL)
        return -2;

    aux2 = aux1;

    i = 1;
    while (aux2->prox != NULL && aux2->prox->caracter == c)
    {
        aux2 = aux2->prox;
        i++;
    }

    aleat = aleatorio(1,i);

    i = 1;
    while (i < aleat)
    {
        aux1 = aux1->prox;
        i++;
    }

    return aux1->codigo;
}

void imprime_codificacao(cabeca_lista_t *lista, FILE *msg_original, FILE *msg_codificada)
{
    wchar_t c;
    int codigo;
    char str_codigo[2 * sizeof(int)];

    c = fgetwc(msg_original);
    while (! feof(msg_original))
    {
        /* procura o codigo do caracter c */
        codigo = busca_caracter_lista(lista,c);

        /* transforma o codigo em string */
        sprintf(str_codigo,"%d ",codigo);

        /* insere no arquivo */
        fputs(str_codigo,msg_codificada);

        c = fgetwc(msg_original);
    }
    fprintf(msg_codificada,"\n");
}