#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "arquivo_chaves.h"

#define TAM_MAX_STRING 100 /* tamanho maximo de cada palavra no livro */
#define INICIO_ASCII 32 /* inicio dos caracteres imprimiveis */
#define FIM_ASCII 126 /* fim dos caracteres imprimiveis */

/* verifica se um caracter esta na lista, retorna 1 se sim */
int contido_lista(cabeca_lista_t *lista, wchar_t j)
{
    nodo_lista_t *aux;

    aux = lista->primeiro;

    while (aux != NULL)
    {
        if (aux->caracter == j)
            return 1;

        aux = aux->prox;
    }

    return 0;
}

/* verifica se um caracter esta na string, retorna 1 se sim */
int contido_string(wchar_t *str, wchar_t c)
{
    int i;

    i = 0;
    while (i < wcslen(str))
    {
        if (str[i] == c)
            return 1;
        i++;
    }

    return 0;
}

/* escreve o conteudo da lista no arquivo de chaves */
void escreve_arquivo(cabeca_lista_t *lista, FILE *arq)
{
    nodo_lista_t *aux = lista->primeiro;

    if (aux == NULL)
        return;

    /* imprime o primeiro nodo */
    fprintf(arq,"%lc: %d ",aux->caracter,aux->codigo);

    /* loop para impressao do resto */
    while (aux->prox != NULL)
    {
        if (aux->prox->caracter != aux->caracter)
            fprintf(arq,"\n%lc: %d ",aux->prox->caracter,aux->prox->codigo);
        else
            fprintf(arq,"%d ",aux->prox->codigo);

        aux = aux->prox;
    }
    fprintf(arq,"\n");
}

/* gera lista a partir do livro cifra */
void gerar_lista_cifra(cabeca_lista_t *lista, FILE *arq_cifras)
{
    wchar_t *str;
    int i; /* codigo */

    /* aloca espaco para a string str, que sera usada para armazenar cada palavra do arq_cifras */
    if (!(str = malloc(TAM_MAX_STRING * sizeof(wchar_t))))
    {
        perror("Erro na alocacao de memoria");
        exit(1);
    }

    /* loop para preencher a lista */
    i = 0;
    while (fscanf(arq_cifras,"%ls",str) != EOF)
    {   
        adiciona_ordem_lista(lista,str[0],i);
        
        i++;
    }

    free(str);
}

/* gera lista a partir do arquivo de chaves */
void gerar_lista_chaves(cabeca_lista_t *lista, FILE *arq_chaves)
{
    wchar_t *str;
    wchar_t caracter; /* caracter do inicio de cada linha */
    int codigo;

    /* aloca espaco para a string str, que sera usada para armazenar cada palavra do arq_chaves */
    if (!(str = malloc(TAM_MAX_STRING * sizeof(wchar_t))))
    {
        perror("Erro na alocacao de memoria");
        exit(1);
    }

    /* loop para preencher a lista */
    fscanf(arq_chaves,"%ls",str);
    while (! feof(arq_chaves))
    {
        if (contido_string(str,':'))
            caracter = str[0];
        else
        {
            /* transforma wchar_t em int */
            codigo = wcstol(str,NULL,10);
            
            adiciona_ordem_lista(lista,caracter,codigo);
        }

        fscanf(arq_chaves,"%ls",str);
    }

    free(str);
}