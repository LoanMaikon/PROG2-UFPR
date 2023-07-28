#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <utime.h>
#include "archive.h"
#include "lista.h"

#define TAM_LEITURA 1024
#define TAM_CAMINHO_RELATIVO 1024

/* a função recebe como parâmetro o archive e o membro que será testado */
/* retorna 1 se membro existe no archive e 0 caso contrário */
int existe(FILE *archive, char *membro)
{
    cabeca_lista_t *lista;
    nodo_lista_t *aux;
    int existe;

    if (vazio(archive))
        return 0;

    lista = cria_lista();

    /* pega as informações já existentes do diretório */
    inicia_lista(lista,archive);

    aux = lista->primeiro;
    while (aux != NULL && strcmp(aux->local,membro) != 0)
        aux = aux->prox;

    if (aux == NULL)
        existe = 0;
    else
        existe = 1;

    lista = destroi_lista(lista);

    return existe;
}

/* a função recebe o archive */
/* retorna o tamanho total do archive*/
long tamanho_archive(FILE *archive)
{
    long tam;

    fseek(archive,0,SEEK_END);
    tam = ftell(archive);

    rewind(archive);

    return tam;
}

/* a função recebe o archive */
/* retorna 1 se arquivo está vazio, 0 caso contrário */
int vazio(FILE *archive)
{
    long tam;

    fseek(archive,0,SEEK_END);
    tam = ftell(archive);

    rewind(archive);

    if (tam == 0)
        return 1;
    return 0;
}

/* a função recebe o archive e o arquivo cujo conteúdo será inserido no archive */
/* insere o conteúdo do membro no archive */
void inserir_conteudo(FILE *archive, FILE *arq)
{
    long pos_dir;
    long tam_arq;
    int tam_arq_resto;
    int i, n;
    char *leitura;

    if (!(leitura = malloc(TAM_LEITURA * sizeof(char))))
    {
        perror("Erro ao alocar memória");
        exit(1);
    }

    /* caso leitura não venha zerada, zera a string */
    if (leitura != NULL)
        memset(leitura,0,TAM_LEITURA * sizeof(char));

    /* lê a posição do diretório */
    fseek(archive,sizeof(long),SEEK_SET);
    fread(&pos_dir,sizeof(long),1,archive);

    /* vê qual é o tamanho do conteúdo do arquivo */
    fseek(arq,0,SEEK_END);
    tam_arq = ftell(arq);

    if (tam_arq == 0)
    {
        free(leitura);
        return;
    }

    /* vê o resto de arq mod 1024 e o número n vezes que 1024 será lido */
    tam_arq_resto = tam_arq % TAM_LEITURA;
    n = tam_arq / TAM_LEITURA;

    /* coloca o ponteiro do archive em pos_dir e dá rewind em arq */
    fseek(archive,pos_dir,SEEK_SET);
    rewind(arq);

    /* escreve no archive */
    i = 0;
    while (i < n)
    {
        fread(leitura,sizeof(char),TAM_LEITURA,arq);
        fwrite(leitura,sizeof(char),TAM_LEITURA,archive);

        i++;
    }
    memset(leitura,0,TAM_LEITURA * sizeof(char));
    fread(leitura,sizeof(char),tam_arq_resto,arq);
    fwrite(leitura,sizeof(char),tam_arq_resto,archive);

    free(leitura);

    rewind(archive);
}

/* recebe o archive */
/* vai até o diretório do archive e trunca ele */
void trunca_diretorio(FILE *archive)
{
    long pos_dir;

    /* lê a posição do diretório */
    fseek(archive,sizeof(long),SEEK_SET);
    fread(&pos_dir,sizeof(long),1,archive);

    /* trunca o archive a partir de pos_dir */
    ftruncate(fileno(archive),pos_dir);

    rewind(archive);
}

/* recebe o archive */
/* coloca valores iniciais no archive (numero de arquivos = 0 e posiçao do diretório = 2 * sizeof(int)) */
void iniciar_archive(FILE *archive)
{
    long n_arqs = 0;
    long pos_dir = 2 * sizeof(long);

    rewind(archive);

    /* número de arquivos */
    fwrite(&n_arqs,sizeof(long),1,archive);

    /* posição do diretório */
    fwrite(&pos_dir,sizeof(long),1,archive);

    rewind(archive);
}

/* recebe o archive e uma lista encadeada */
/* atualiza a posição do diretório e o número de arquivos, calcula pela lista a nova posição do diretório */
void atualiza_inicio(FILE *archive, cabeca_lista_t *lista)
{
    long n_arqs = 0;
    long pos_dir;
    nodo_lista_t *aux;

    aux = lista->primeiro;

    if (aux == NULL)
        return;
    
    while (aux != NULL)
    {
        n_arqs++;
        aux = aux->prox;
    }
    
    /* escreve o novo n_arqs no archive */
    rewind(archive);
    fwrite(&n_arqs,sizeof(long),1,archive);

    /* calcula a nova posição do diretório */
    pos_dir = tamanho_conteudo(lista) + 2 * sizeof(long);

    /* escreve a nova posição do diretório */
    fwrite(&pos_dir,sizeof(long),1,archive);

    rewind(archive);
}

/* recebe o archive, uma lista encadeada e o membro cujo conteúdo seja retirado do archive */
/* remove o conteúdo do membro, truncando o excesso */
void remove_membro_conteudo(cabeca_lista_t *lista, FILE *archive, char *membro)
{
    nodo_lista_t *aux;
    int ordem;
    long memoria_antes = 0; /* memória antes do conteúdo do membro */
    long memoria_depois = 0; /* memória antes do conteúdo do membro + memória do membro */
    long memoria_archive = tamanho_archive(archive);
    int n;
    int tam_archive_resto;
    int i;
    int tam_membro;
    long memoria_truncar;
    char *leitura;

    if (!(leitura = malloc(TAM_LEITURA * sizeof(char))))
    {
        perror("Erro ao alocar memória");
        exit(1);
    }

    /* caso leitura não venha zerada, zera a string */
    if (leitura != NULL)
        memset(leitura,0,TAM_LEITURA * sizeof(char));
    
    /* pegando a memória do membro na lista */
    aux = lista->primeiro;
    while (aux != NULL && strcmp(aux->local,membro) != 0)
        aux = aux->prox;
    
    /* não encontrou o membro */
    if (aux == NULL)
        return;

    ordem = aux->ordem;
    memoria_depois += aux->tamanho; /* soma tamanho do membro */
    tam_membro = aux->tamanho;

    /* calculando memória do conteúdo que vem antes do membro */
    aux = lista->primeiro;
    while (aux != NULL)
    {
        if (aux->ordem < ordem)
            memoria_antes += aux->tamanho;

        aux = aux->prox;
    }
    memoria_depois += memoria_antes; /* soma o que vem antes de membro */

    /* soma os dados iniciais (n_arqs e pos_dir) */
    memoria_antes += (2 * sizeof(long));
    memoria_depois += (2 * sizeof(long));

    /* vê o resto de arq mod 1024 e o número n vezes que 1024 será lido */
    tam_archive_resto = (memoria_archive - memoria_depois) % TAM_LEITURA;
    n = (memoria_archive - memoria_depois) / TAM_LEITURA;

    /* escreve no archive */
    i = 0;
    while (i < n)
    {
        fseek(archive,memoria_depois,SEEK_SET);
        fread(leitura,sizeof(char),TAM_LEITURA,archive);
        fseek(archive,memoria_antes,SEEK_SET);
        fwrite(leitura,sizeof(char),TAM_LEITURA,archive);

        /* atualizando memórias */
        memoria_antes += TAM_LEITURA;
        memoria_depois += TAM_LEITURA;

        i++;
    }
    memset(leitura,0,TAM_LEITURA * sizeof(char));

    fseek(archive,memoria_depois,SEEK_SET);
    fread(leitura,sizeof(char),tam_archive_resto,archive);
    fseek(archive,memoria_antes,SEEK_SET);
    fwrite(leitura,sizeof(char),tam_archive_resto,archive);

    /* truncando excesso do arquivo */
    memoria_truncar = memoria_archive - tam_membro;
    ftruncate(fileno(archive),memoria_truncar); /* trunca a partir de memoria_truncar */


    free(leitura);
    rewind(archive);
}

/* recebe o archive, uma lista encadeada e um membro que será criado seguindo hierarquia de diretórios */
/* descompacta o membro */
void cria_membro(cabeca_lista_t *lista, FILE *archive, char *membro)
{
    nodo_lista_t *aux1, *aux2;
    long pos_membro = 0;
    int ordem_membro;
    char nome[TAM_NOME];
    int i;
    char *leitura;
    char *dir;
    char *temp;
    int tam_arq_resto;
    int n;
    FILE *arq;
    char nome_temp[TAM_LOCALIZACAO];

    if (!(leitura = malloc(TAM_LEITURA * sizeof(char))))
    {
        perror("Erro ao alocar memória");
        exit(1);
    }

    /* caso leitura não venha zerada, zera a string */
    if (leitura != NULL)
        memset(leitura,0,TAM_LEITURA * sizeof(char));

    strcpy(nome_temp,membro);

    /* acha o nodo onde o membro está */
    aux1 = lista->primeiro;
    while (aux1 != NULL && strcmp(aux1->local,membro) != 0)
        aux1 = aux1->prox;

    if (aux1 == NULL)
        return;

    strcpy(nome,aux1->nome);
    ordem_membro = aux1->ordem;

    /* calcula a posição do membro no archive */
    pos_membro += 2 * sizeof(long);
    i = 1;
    while (i < ordem_membro)
    {
        aux2 = lista->primeiro;
        while (aux2->ordem != i)
            aux2 = aux2->prox;

        pos_membro += aux2->tamanho;

        i++;
    }

    /* aponta para o diretório do membro */
    temp = strrchr(nome_temp,'/');
    if (temp != NULL)
        *temp = '\0'; 

    dir = strtok(nome_temp,"/");
    while (dir != NULL)
    {
        chdir(dir);
        dir = strtok(NULL,"/");
    }

    /* cria o arquivo */
    arq = fopen(nome,"w+");

    /* vê o resto de arq mod 1024 e o número n vezes que 1024 será lido */
    tam_arq_resto = (aux1->tamanho) % TAM_LEITURA;
    n = (aux1->tamanho) / TAM_LEITURA;

    /* escreve no archive */
    fseek(archive,pos_membro,SEEK_SET);
    i = 0;
    while (i < n)
    {
        fread(leitura,sizeof(char),TAM_LEITURA,archive);
        fwrite(leitura,sizeof(char),TAM_LEITURA,arq);

        i++;
    }
    memset(leitura,0,TAM_LEITURA * sizeof(char));

    fread(leitura,sizeof(char),tam_arq_resto,archive);
    fwrite(leitura,sizeof(char),tam_arq_resto,arq);

    /* arrumando as permissões do arquivo */
    chmod(nome,aux1->permissoes);

    /* arrumando o UID do arquivo */
    chown(nome,aux1->uid,-1);

    fclose(arq);
    free(leitura);
}