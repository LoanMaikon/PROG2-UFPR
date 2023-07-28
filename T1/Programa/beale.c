#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>
#include <getopt.h>
#include <wchar.h>
#include "lista.h"
#include "arquivo_chaves.h"
#include "codificacao.h"
#include "decodificacao.h"

void decodifica(FILE *msg_codificada, FILE *msg_decodificada, FILE *arq_cifras, FILE *arq_chaves)
{
    cabeca_lista_t *lista;

    /* aloca espaco para a lista */
    lista = cria_lista();

    /* preenche a lista com os caracteres e os codigos */
    if (arq_cifras)
        gerar_lista_cifra(lista,arq_cifras);
    else if (arq_chaves)
        gerar_lista_chaves(lista,arq_chaves);

    /* preenche o arquivo msg_decodificada */
    imprime_decodificacao(lista,msg_codificada,msg_decodificada);

    /* desaloca memoria da lista */
    lista = destroi_lista(lista);
}

void codifica(FILE *arq_cifras, FILE *msg_codificada, FILE *msg_original, FILE *arq_chaves)
{
    cabeca_lista_t *lista;

    /* aloca espaco para a lista */
    lista = cria_lista();

    /* preenche a lista com os caracteres e os codigos a partir do livro cifra */
    gerar_lista_cifra(lista,arq_cifras);

    /* se arq_chaves foi aberto, entao escreve nele */
    if (arq_chaves)
        escreve_arquivo(lista,arq_chaves);

    /* preenche o arquivo msg_codificada */
    imprime_codificacao(lista,msg_original,msg_codificada);

    /* desaloca memoria da lista */
    lista = destroi_lista(lista);
}

void aplica_operacao(int num_args, char **args)
{
    char parametro;
    FILE *arq_chaves = NULL;
    FILE *msg_codificada = NULL;
    FILE *msg_decodificada = NULL;
    FILE *msg_original = NULL;
    FILE *arq_cifras = NULL;

    parametro = getopt(num_args,args,"de:");

    if (parametro == 'd')
    {
        optind--;

        while ((parametro = getopt(num_args,args,"di:b:c:o:")) != -1)
            switch(parametro)
            {
                case 'b':
                    arq_cifras = fopen(optarg,"r");

                    if (! arq_cifras)
                    {
                        perror("Erro ao abrir arquivo de chaves");
                        exit(1);
                    }

                    break;
                case 'c':
                    arq_chaves = fopen(optarg,"r");

                    if (! arq_chaves)
                    {
                        perror("Erro ao abrir arquivo de chaves");
                        exit(1);
                    }

                    break;        
                case 'i':
                    msg_codificada = fopen(optarg,"r");
                    break;
                case 'o':
                    msg_decodificada = fopen(optarg,"w");
                    break;
                default:
                    break;
            }
        
        if (! msg_codificada)
        {
            perror("Erro ao abrir arquivo da mensagem codificada");
            exit(1);
        }

        if (! msg_decodificada)
        {
            perror("Erro ao abrir arquivo da mensagem decodificada");
            exit(1);
        }

        /* operacao de decodificacao */
        if (arq_cifras || arq_chaves)
            decodifica(msg_codificada,msg_decodificada,arq_cifras,arq_chaves);
        else
        {
        perror("Entrada invalida");
        exit(1);
        }

        /* fecha arquivos que foram abertos */
        fclose(msg_codificada);
        fclose(msg_decodificada);
        if (arq_cifras)
            fclose(arq_cifras);
        if (arq_chaves)
            fclose(arq_chaves);
    }
    else
    if (parametro == 'e')
    {
        optind--;
        
        while ((parametro = getopt(num_args,args,"eb:m:o:c:")) != -1)
            switch(parametro)
            {
                case 'b':
                    arq_cifras = fopen(optarg,"r");
                    break;
                case 'c':
                    arq_chaves = fopen(optarg,"w");

                    if (! arq_chaves)
                    {
                        perror("Erro ao abrir arquivo de chaves");
                        exit(1);
                    }

                    break;        
                case 'm':
                    msg_original = fopen(optarg,"r");
                    break;
                case 'o':
                    msg_codificada = fopen(optarg,"w");
                    break;
                default:
                    break;
            }

        if (! arq_cifras)
        {
            perror("Erro ao abrir arquivo de cifras");
            exit(1);
        }

        if (! msg_original)
        {
            perror("Erro ao abrir arquivo da mensagem original");
            exit(1);
        }

        if (! msg_codificada)
        {
            perror("Erro ao abrir arquivo da mensagem codificada");
            exit(1);
        }

        /* operacao de codificacao */
        codifica(arq_cifras,msg_codificada,msg_original,arq_chaves);

        /* fecha arquivos que foram abertos */
        fclose(arq_cifras);
        fclose(msg_codificada);
        fclose(msg_original);
        if (arq_chaves)
            fclose(arq_chaves);
    }
    else
    {
        perror("Entrada invalida");
        exit(1);
    }
}

int main(int num_args, char **args)
{
    setlocale(LC_ALL,"pt_BR.UTF-8");
    srand(time(NULL));

    /* verifica qual operacao esta sendo feita e se entradas estao certas, tambem aplica */
    aplica_operacao(num_args,args);

    return 0;
}