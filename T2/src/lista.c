#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include "lista.h"

#define TAM_NOME 1024
#define TAM_LOCALIZACAO 1024
#define TAM_CAMINHO_RELATIVO 1024

/* aloca memória para a lista e retorna ela */
cabeca_lista_t *cria_lista()
{
    cabeca_lista_t *lista;

    if (!(lista = malloc(sizeof(cabeca_lista_t))))
        return NULL;

    lista->primeiro = NULL;

    return lista;
}

/* recebe uma lista encadeada */
/* desaloca a memória da lista */
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

/* recebe o membro cujo nome será retornado */
/* retorna o nome do arquivo "membro" dentro do caminho relativo */
char *acha_nome(char *membro)
{
    char *nome = strrchr(membro,'/');

    if (nome == NULL)
        return membro;
    else
        return nome + 1;
}

/* recebe o archive, uma lista encadeada e o membro de cujas informações serão retiradas */
/* aloca espaço para um novo nodo e coloca as informações nele a partir do membro */
nodo_lista_t *cria_nodo(cabeca_lista_t *lista, char *membro, FILE *archive)
{
    struct stat info;
    nodo_lista_t *nodo;
    long ordem;

    if (!(nodo = malloc(sizeof(nodo_lista_t))))
    {
        perror("Erro ao alocar memória para nodo");
        exit(1);
    }

    /* aloca informações stat em info */
    stat(membro,&info);

    /* pegando a ordem do nodo */
    fseek(archive,0,SEEK_SET);
    fread(&ordem,sizeof(long),1,archive);
    ordem++;

    /* copiando informações */
    nodo->uid = info.st_uid;
    nodo->permissoes = info.st_mode;
    nodo->tamanho = info.st_size;
    nodo->data_mod = info.st_mtime;
    nodo->ordem = ordem;
    strcpy(nodo->local,membro);
    strcpy(nodo->nome,acha_nome(membro));
    nodo->gid = info.st_gid;

    nodo->prox = NULL;

    return nodo;
}

/* recebe o archive, uma lista encadeada e o membro que será inserido na lista */
/* adiciona à lista o membro passado na linha de comando */
void adiciona_membro_lista(FILE *archive, cabeca_lista_t *lista, char *membro)
{
    nodo_lista_t *nodo, *aux;

    /* preenche o nodo com as informações do membro */
    nodo = cria_nodo(lista,membro,archive);

    if (lista->primeiro == NULL)
    {
        lista->primeiro = nodo;
        return;
    }

    aux = lista->primeiro;
    while (aux->prox != NULL)
        aux = aux->prox;
    
    aux->prox = nodo;
}

/* recebe o archive e a lista encadeada que será escrita no archive */
/* escreve lista no diretório do archive */
void escreve_lista(cabeca_lista_t *lista, FILE *archive)
{
    nodo_lista_t *aux;
    long n_arqs, pos_dir;
    int i;

    /* lê número de arquivos e posição do diretório do archive */
    rewind(archive);
    fread(&n_arqs,sizeof(long),1,archive);
    fread(&pos_dir,sizeof(long),1,archive);

    /* coloca o ponteiro do archive para o diretório */
    fseek(archive,pos_dir,SEEK_SET);

    aux = lista->primeiro;

    i = 0;
    while (i < n_arqs)
    {
        fwrite(&aux->nome,sizeof(char),TAM_NOME,archive);
        fwrite(&aux->uid,sizeof(uid_t),1,archive);
        fwrite(&aux->permissoes,sizeof(mode_t),1,archive);
        fwrite(&aux->tamanho,sizeof(off_t),1,archive);
        fwrite(&aux->data_mod,sizeof(time_t),1,archive);
        fwrite(&aux->ordem,sizeof(int),1,archive);
        fwrite(&aux->local,sizeof(char),TAM_LOCALIZACAO,archive);
        fwrite(&aux->gid,sizeof(gid_t),1,archive);

        aux = aux->prox;
        i++;
    }
}

/* recebe uma lista encadeada e informações que serão adicionadas a um nodo nessa lista */
/* recebe informações de um arquivo e adiciona essas informações na lista */
void adiciona_nodo(cabeca_lista_t *lista, char nome[TAM_NOME], uid_t uid, mode_t permissoes,
off_t tamanho, time_t data_mod, int ordem, char local[TAM_LOCALIZACAO], gid_t gid)
{
    nodo_lista_t *nodo, *aux;

    if (!(nodo = malloc(sizeof(nodo_lista_t))))
    {
        perror("Erro ao alocar memória para nodo");
        exit(1);
    }

    /* passando informações ao nodo */
    nodo->prox = NULL;
    strcpy(nodo->nome,acha_nome(nome));
    nodo->uid = uid;
    nodo->permissoes = permissoes;
    nodo->tamanho = tamanho;
    nodo->data_mod = data_mod;
    nodo->ordem = ordem;
    strcpy(nodo->local,local);
    nodo->gid = gid;

    /* se lista->primeiro == NULL, adiciona nodo e retorna */
    if (lista->primeiro == NULL)
    {
        lista->primeiro = nodo;
        return;
    }

    /* caso contrário, acha o nodo ao qual será ligado */
    aux = lista->primeiro;
    while (aux->prox != NULL)
        aux = aux->prox;
    
    aux->prox = nodo;
}

/* recebe o archive e uma lista na qual serão colocadas as informações já existentes no archive */
/* vai até a área de diretório do archive e pega as informações já existentes */
void inicia_lista(cabeca_lista_t *lista, FILE *archive)
{
    long n_arqs;
    long pos_dir;
    int i;
    char nome[TAM_NOME];
    uid_t uid;
    mode_t permissoes;
    off_t tamanho;
    time_t data_mod;
    int ordem;
    char local[TAM_LOCALIZACAO];
    int tam;
    gid_t gid;

    /* verifica se archive está vazio */ 
    fseek(archive,0,SEEK_END);
    tam = ftell(archive);
    if (tam == 0)
        return;

    /* lê número de arquivos e posição do diretório do archive */
    rewind(archive);
    fread(&n_arqs,sizeof(long),1,archive);
    fread(&pos_dir,sizeof(long),1,archive);

    /* se não tem diretório, retorna sem nada */
    if (n_arqs == 0)
        return;
    
    /* coloca o ponteiro de archive para o início do diretório */
    fseek(archive,pos_dir,SEEK_SET);

    i = 0;
    while (i < n_arqs)
    {
        /* lendo informações do archive */
        fread(nome,sizeof(char),TAM_NOME,archive);
        fread(&uid,sizeof(uid_t),1,archive);
        fread(&permissoes,sizeof(mode_t),1,archive);
        fread(&tamanho,sizeof(off_t),1,archive);
        fread(&data_mod,sizeof(time_t),1,archive);
        fread(&ordem,sizeof(int),1,archive);
        fread(local,sizeof(char),TAM_LOCALIZACAO,archive);
        fread(&gid,sizeof(gid_t),1,archive);

        /* adiciona à lista um nodo com as informações adquiridas */
        adiciona_nodo(lista,nome,uid,permissoes,tamanho,data_mod,ordem,local,gid);
        
        i++;
    }
}

/* recebe uma lista */
/* retorna o tamanho do conteúdo do archive */
long tamanho_conteudo(cabeca_lista_t *lista)
{
    nodo_lista_t *aux;
    long tam = 0;

    aux = lista->primeiro;

    while (aux != NULL)
    {
        tam += aux->tamanho;

        aux = aux->prox;
    }

    return tam;
}

/* recebe uma lista encadeada e o membro que será retirado da lista */
/* remove um membro da lista e desaloca sua memória */
void remove_membro_lista(cabeca_lista_t *lista, char *membro)
{
    nodo_lista_t *aux1, *aux2, *aux3;
    int i;

    /* membro é o primeiro da lista */
    aux1 = lista->primeiro;
    if (strcmp(aux1->local,membro) == 0)
    {
        lista->primeiro = aux1->prox;
        aux1->prox = NULL;
        free(aux1);

        /* atualiza ordens */
        aux1 = lista->primeiro;
        i = 1;
        while (aux1 != NULL)
        {
            aux1->ordem = i;
            i++;
            aux1 = aux1->prox;
        }

        return;
    }

    /* membro está no meio da lista */
    aux1 = lista->primeiro;
    while (aux1->prox != NULL && strcmp(aux1->prox->local,membro) != 0)
        aux1 = aux1->prox;
    aux2 = aux1->prox;

    /* atualiza ordens */
    aux3 = lista->primeiro;
    while (aux3 != NULL)
    {
        if (aux3->ordem > aux2->ordem)
            aux3->ordem--;
        aux3 = aux3->prox;
    }

    aux1->prox = aux2->prox;
    free(aux2);
}

/* recebe o archive e o membro do qual será retirada a data de modificação nova */
/* retorna 1 se data de modificação do arquivo novo é maior que a do antigo, 0 caso contrário */
int compara_datamod(FILE *archive, char *membro)
{
    struct stat info;
    FILE *arq;
    int datamod_novo, datamod_antigo;
    nodo_lista_t *aux;
    cabeca_lista_t *lista;

    /* aloca espaço para a lista */
    lista = cria_lista();

    /* pega as informações já existentes do diretório */
    inicia_lista(lista,archive);

    /* aloca informações stat em info e pega o datamod_novo do membro */
    arq = fopen(membro,"r");
    if (! arq)
    {
        perror("Erro ao alocar memória para nodo");
        exit(1);
    }
    stat(membro,&info);
    datamod_novo = info.st_mtime;;

    /* pega o datamod_antigo da lista */
    aux = lista->primeiro;
    while (aux != NULL && strcmp(aux->local,membro) != 0)
        aux = aux->prox;
    
    datamod_antigo = aux->data_mod;

    lista = destroi_lista(lista);

    fclose(arq);

    if (datamod_novo > datamod_antigo)
        return 1;
    return 0;
}

/* recebe uma lista, um target e um membro que será colocado depois de target na lista */
/* muda o membro para depois do target */
void mover_membro(cabeca_lista_t *lista, char *membro, char *target)
{
    nodo_lista_t *aux1, *aux2, *aux3, *aux4, *aux5;
    int ordem_target = 1; /* ordem na lista */
    int ordem_membro = 1; /* ordem na lista */

    /* membro == target */
    if (strcmp(membro,target) == 0)
    {
        return;
    }

    /* pegando a ordem do target */
    aux1 = lista->primeiro;
    while (aux1 != NULL && strcmp(aux1->local,target) != 0)
    {
        ordem_target++;
        aux1 = aux1->prox;
    }

    if (aux1 == NULL)
    {
        return;
    }

    /* pegando a ordem do membro */
    aux2 = lista->primeiro;
    while (aux2 != NULL && strcmp(aux2->local,membro) != 0)
    {
        ordem_membro++;
        aux2 = aux2->prox;
    }

    if (aux2 == NULL)
    {
        return;
    }

    if (ordem_membro == ordem_target + 1)
    {
        return;
    }
    
    if (ordem_membro > ordem_target)
    {
        /* colocando aux5 no nodo anterior ao membro */
        aux5 = aux1;
        while (aux5->prox != aux2)
            aux5 = aux5->prox;

        aux3 = aux1->prox;
        aux4 = aux2->prox;
        aux1->prox = aux2;
        aux2->prox = aux3;
        aux5->prox = aux4;
    }

    else if (ordem_membro < ordem_target)
    {
        /* colocando aux5 no nodo anterior ao membro */
        aux5 = lista->primeiro;

        aux3 = aux1->prox;
        aux4 = aux2->prox;
        
        if (aux5 != aux2)
        {
            while (aux5->prox != aux2)
                aux5 = aux5->prox;

            aux1->prox = aux2;
            aux5->prox = aux4;
            aux2->prox = aux3;
        }
        else
        {
            aux1->prox = aux2;
            lista->primeiro = aux2->prox;
            aux2->prox = aux3;
        }
    }
}

/* recebe uma lista encadeada */
/* imprime o que está na lista no diretório do archive */
void imprime_lista(cabeca_lista_t *lista)
{
    char data_mod[20];
    nodo_lista_t *aux;
    struct passwd *pw;
    struct group *gr;
    char *nome;
    char *grupo;
    char permissoes[12];

    aux = lista->primeiro;

    while (aux != NULL)
    {
        /* usuário proprietário do arquivo */
        pw = getpwuid(aux->uid);
        nome = pw->pw_name;

        /* grupo proprietário do arquivo */
        gr = getgrgid(aux->gid);
        grupo = gr->gr_name;

        /* data de modificação */
        struct tm *tm = localtime(&aux->data_mod);
        strftime(data_mod, sizeof(data_mod), "%Y-%m-%d %H:%M", tm);

        /* permissões */
        snprintf(permissoes, sizeof(permissoes), "%c%c%c%c%c%c%c%c%c%c%c",
            (S_ISDIR(aux->permissoes) ? 'd' : '-'),
            (aux->permissoes & S_IRUSR) ? 'r' : '-',
            (aux->permissoes & S_IWUSR) ? 'w' : '-',
            (aux->permissoes & S_IXUSR) ? 'x' : '-',
            (aux->permissoes & S_IRGRP) ? 'r' : '-',
            (aux->permissoes & S_IWGRP) ? 'w' : '-',
            (aux->permissoes & S_IXGRP) ? 'x' : '-',
            (aux->permissoes & S_IROTH) ? 'r' : '-',
            (aux->permissoes & S_IWOTH) ? 'w' : '-',
            (aux->permissoes & S_IXOTH) ? 'x' : '-','\0');

        /* imprimindo as informações */
        printf("%s ",permissoes);
        printf("%s/%s  ",nome,grupo);
        printf("%8ld ",aux->tamanho);
        printf("%s ",data_mod);
        printf("%s\n",aux->nome);

        aux = aux->prox;
    }
}

/* recebe o membro que contém a hierarquia de diretórios */
/* cria os diretórios passados por membro (caminho relativo) */
void cria_diretórios(char *membro)
{
    char *dir;
    struct stat info;
    char *nome;
    char temp[TAM_LOCALIZACAO];

    strcpy(temp,membro);
 
    /* excluindo o nome do arquivo do membro */
    nome = strrchr(temp,'/');
    if (nome != NULL)
        *nome = '\0'; 

    /* loop para verificar se diretório existe, se não cria */
    dir = strtok(temp,"/");
    while (dir != NULL)
    {
        /* se diretório não existe, cria */
        if (stat(dir,&info) == -1)
            mkdir(dir, 0700);
        
        chdir(dir);

        dir = strtok(NULL,"/");
    }
}