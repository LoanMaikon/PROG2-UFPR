#include "opt.h"
#include "lista.h"
#include "archive.h"
#include <stdio.h>
#include <stdlib.h>

void opt_a(FILE *archive, char *membro)
{
    cabeca_lista_t *lista;
    FILE *arq;

    arq = fopen(membro,"r");
    if (! arq)
    {
        printf("Erro ao abrir arquivo %s\n",membro);
        return;
    }

    /* se membro já existe, compara as datas de modificação */
    if (existe(archive,membro))
    {
        if (compara_datamod(archive,membro) > 0)
            opt_r(archive,membro);
        else /* se a data de modificação for menor ou igual, simplesmente retorna */
        {
            fclose(arq);
            return;
        }
    }

    /* se o arquivo está vazio, escreve informações iniciais nele */
    if (vazio(archive))
        iniciar_archive(archive); /* escreve número de arquivos e posição do diretório */

    /* aloca espaço para a lista */
    lista = cria_lista();

    /* pega as informações já existentes do diretório */
    inicia_lista(lista,archive);

    /* trunca o diretório para inserção do conteúdo do novo arquivo */
    trunca_diretorio(archive);

    /* insere o conteúdo do membro no archive, coloca conteúdo no pos_dir */
    inserir_conteudo(archive,arq);

    /* adiciona as informações do novo membro */
    adiciona_membro_lista(archive,lista,membro);

    /* atualiza a posição do diretório e o número de arquivos */
    atualiza_inicio(archive,lista);

    /* escreve lista no diretório */
    escreve_lista(lista,archive);

    /* desaloca a memória da lista */
    lista = destroi_lista(lista);

    fclose(arq);
}

void opt_c(FILE *archive)
{
    cabeca_lista_t *lista;

    if (vazio(archive))
    {
        printf("Archive vazio\n");
        return;
    }

    /* aloca espaço para a lista */
    lista = cria_lista();

    /* pega as informações já existentes do diretório */
    inicia_lista(lista,archive);

    /* imprimindo as informações da lista no stdout */
    imprime_lista(lista);

    /* desaloca a memória da lista */
    lista = destroi_lista(lista);
}

void opt_h()
{
    printf("O programa deve ser executado da seguinte forma:\n");
    printf("./vina++ <opção> <archive> [membro1 membro2 ...]\n\n");
    printf("Opção i: insere/acrescenta um ou mais membros ao archive. Se um membro já existir, substitui\n");
    printf("Opção a: insere/atualiza um membro. Caso ele já exista, atualiza se parâmetro for mais recente.\n");
    printf("Opção m <target>: move o parâmetro para depois de target no archive.\n");
    printf("Opção x: extrai os membros indicados do archive.\n");
    printf("Opção r: remove os membros indicados do archive.\n");
    printf("Opção c: lista o conteúdo do archive.\n");
    printf("Opção z: limpa o archive\n");
    printf("Opção h: lista as opções de comandos.\n");
}

void opt_i(FILE *archive, char *membro)
{
    cabeca_lista_t *lista;
    FILE *arq;

    arq = fopen(membro,"r");
    if (! arq)
    {
        printf("Erro ao abrir arquivo %s\n",membro);
        return;
    }

    /* se membro já existe no archive, exclui */
    if (existe(archive,membro))
        opt_r(archive,membro);

    if (vazio(archive))
        iniciar_archive(archive); /* escreve número de arquivos e posição do diretório */

    /* aloca espaço para a lista */
    lista = cria_lista();

    /* pega as informações já existentes do diretório */
    inicia_lista(lista,archive);

    /* trunca o diretório para inserção do conteúdo do novo arquivo */
    trunca_diretorio(archive);

    /* insere o conteúdo do membro no archive, coloca conteúdo no pos_dir */
    inserir_conteudo(archive,arq);

    /* adiciona as informações do novo membro */
    adiciona_membro_lista(archive,lista,membro);

    /* atualiza a posição do diretório e o número de arquivos */
    atualiza_inicio(archive,lista);

    /* escreve lista no diretório */
    escreve_lista(lista,archive);

    /* desaloca a memória da lista */
    lista = destroi_lista(lista);

    fclose(arq);
}

void opt_m(FILE *archive, char *membro, char *target)
{
    cabeca_lista_t *lista;

    /* archive vazio */
    if (vazio(archive))
        return;
    
    /* target não existe */
    if (! existe(archive,target))
        return;
    
    /* membro não existe */
    if (! existe(archive,membro))
        return;

    /* aloca espaço para a lista */
    lista = cria_lista();

    /* pega as informações já existentes do diretório */
    inicia_lista(lista,archive);

    /* trunca o diretório para inserção do conteúdo do novo arquivo */
    trunca_diretorio(archive);

    /* operação para mover o membro */
    mover_membro(lista,membro,target);

    /* escreve lista no diretório */
    escreve_lista(lista,archive);

    /* desaloca a memória da lista */
    lista = destroi_lista(lista);
}

void opt_r(FILE *archive, char *membro)
{
    cabeca_lista_t *lista;

    if (vazio(archive))
    {
        printf("Archive vazio\n");
        return;
    }

    if (! existe(archive,membro))
    {   
        printf("%s não existe no archive\n",membro);
        return;
    }

    /* aloca espaço para a lista */
    lista = cria_lista();

    /* pega as informações já existentes do diretório */
    inicia_lista(lista,archive);

    if (lista->primeiro->prox == NULL)
    {
        lista = destroi_lista(lista);
        opt_z(archive);
        return;
    }

    /* trunca o diretório para remover o conteúdo do arquivo */
    trunca_diretorio(archive);

    /* remove o membro do conteúdo do archive */
    remove_membro_conteudo(lista,archive,membro);

    /* remove membro da lista */
    remove_membro_lista(lista,membro);

    /* atualiza a posição do diretório e o número de arquivos */
    atualiza_inicio(archive,lista);

    /* escreve lista no diretório */
    escreve_lista(lista,archive);

    /* desaloca a memória da lista */
    lista = destroi_lista(lista);
}

void opt_x(FILE *archive, char *membro)
{
    cabeca_lista_t *lista;

    if (vazio(archive))
    {
        printf("Archive vazio\n");
        return;
    }

    if (! existe(archive,membro))
    {   
        printf("%s não existe no archive\n",membro);
        return;
    }

    /* aloca espaço para a lista */
    lista = cria_lista();

    /* pega as informações já existentes do diretório */
    inicia_lista(lista,archive);

    /* cria a hierarquia de diretórios de membro caso ela não exista */
    cria_diretórios(membro);

    /* cria o arquivo membro */
    cria_membro(lista,archive,membro);

    /* desaloca a memória da lista */
    lista = destroi_lista(lista);
}

void opt_x_tudo(FILE *archive)
{
    cabeca_lista_t *lista;
    nodo_lista_t *aux;

    if (vazio(archive))
    {
        printf("Archive vazio\n");
        return;
    }

    /* aloca espaço para a lista */
    lista = cria_lista();

    /* pega as informações já existentes do diretório */
    inicia_lista(lista,archive);

    /* percorre a lista fazendo o mesmo processo de opt_x (usando opt_x para cada localização) */
    aux = lista->primeiro;
    while (aux != NULL)
    {
        opt_x(archive,aux->local);

        aux = aux->prox;
    }

    /* desaloca a memória da lista */
    lista = destroi_lista(lista);
}

void opt_z(FILE *archive)
{
    ftruncate(fileno(archive),0);
}