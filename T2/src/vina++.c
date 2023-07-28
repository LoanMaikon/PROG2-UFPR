#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "lista.h"
#include "archive.h"
#include "opt.h"

#define TAM_CAMINHO_RELATIVO 1024

/* retorna o caminho relativo do membro */
char *caminho_relativo(char *caminho, char *membro)
{
    memset(caminho,0,TAM_CAMINHO_RELATIVO * sizeof(char));
    
    if (membro[0] == '.' && membro[1] == '/')
        strcpy(caminho,membro);
           
    else if (membro[0] == '/')
        sprintf(caminho,".%s",membro);

    else
        sprintf(caminho,"./%s",membro);

    return caminho;
}

int main(int argc, char **argv)
{
    char parametro;
    FILE *archive = NULL;
    int i;
    char *caminho_membro;
    char *caminho_target;

    if (!(caminho_membro = malloc(TAM_CAMINHO_RELATIVO * sizeof(char))))
    {
        perror("Erro ao alocar memória\n");
        exit(1);
    }

    if (!(caminho_target = malloc(TAM_CAMINHO_RELATIVO * sizeof(char))))
    {
        perror("Erro ao alocar memória\n");
        free(caminho_membro);
        exit(1);
    }

    parametro = getopt(argc,argv,"i:a:m:x:r:c:hz:");

    switch(parametro)
    {
        case 'i':
            caminho_membro = caminho_relativo(caminho_membro,optarg);
            archive = fopen(caminho_membro,"r+");

            if (! archive)
                archive = fopen(caminho_membro,"w+");

            if (! archive)
            {
                perror("Erro ao abrir archive");
                free(caminho_membro);
                free(caminho_target);
                exit(1);
            }

            i = optind; 
            while (i < argc)
            {
                caminho_membro = caminho_relativo(caminho_membro,argv[i]);
                opt_i(archive,caminho_membro);
                i++;
            }

            fclose(archive);

            break;

        case 'a':
            caminho_membro = caminho_relativo(caminho_membro,optarg);
            archive = fopen(caminho_membro,"r+");

            if (! archive)
                archive = fopen(caminho_membro,"w+");

            if (! archive)
            {
                perror("Erro ao abrir archive");
                free(caminho_membro);
                free(caminho_target);
                exit(1);
            }

            i = optind;
            while (i < argc)
            {
                caminho_membro = caminho_relativo(caminho_membro,argv[i]);
                opt_a(archive,caminho_membro);
                i++;
            }

            fclose(archive);
            break;

        case 'm':
            caminho_target = caminho_relativo(caminho_target,optarg);

            i = optind;

            caminho_membro = caminho_relativo(caminho_membro,argv[i]);
            archive = fopen(caminho_membro,"r+");

            if (! archive)
                archive = fopen(caminho_membro,"w+");

            if (! archive)
            {
                perror("Erro ao abrir archive");
                free(caminho_membro);
                free(caminho_target);
                exit(1);
            }

            i++;
            while (i < argc)
            {
                caminho_membro = caminho_relativo(caminho_membro,argv[i]);
                opt_m(archive,caminho_membro,caminho_target);
                i++;
            }

            fclose(archive);
            break;

        case 'x':
            caminho_membro = caminho_relativo(caminho_membro,optarg);
            
            archive = fopen(caminho_membro,"r+");

            if (! archive)
                archive = fopen(caminho_membro,"w+");

            if (! archive)
            {
                perror("Erro ao abrir archive");
                free(caminho_membro);
                free(caminho_target);
                exit(1);
            }

            i = optind;
            if (! argv[i])
                opt_x_tudo(archive);
            else
            {
                while (i < argc)
                {

                    caminho_membro = caminho_relativo(caminho_membro,argv[i]);
                    opt_x(archive,caminho_membro);
                    i++;
                }
            }

            fclose(archive);
            break;

        case 'r':
            caminho_membro = caminho_relativo(caminho_membro,optarg);
            archive = fopen(caminho_membro,"r+");

            if (! archive)
                archive = fopen(caminho_membro,"w+");

            if (! archive)
            {
                perror("Erro ao abrir archive");
                free(caminho_membro);
                free(caminho_target);
                exit(1);
            }

            i = optind;
            while (i < argc)
            {
                caminho_membro = caminho_relativo(caminho_membro,argv[i]);
                opt_r(archive,caminho_membro);
                i++;
            }

            fclose(archive);
            break;

        case 'c':
            caminho_membro = caminho_relativo(caminho_membro,optarg);
            archive = fopen(caminho_membro,"r+");

            if (! archive)
                archive = fopen(caminho_membro,"w+");

            if (! archive)
            {
                perror("Erro ao abrir archive");
                free(caminho_membro);
                free(caminho_target);
                exit(1);
            }

            opt_c(archive);

            fclose(archive);
            break;
        
        case 'h':
            opt_h();

            break;

        case 'z':
            caminho_membro = caminho_relativo(caminho_membro,optarg);
            archive = fopen(caminho_membro,"r+");

            if (! archive)
                archive = fopen(caminho_membro,"w+");

            if (! archive)
            {
                perror("Erro ao abrir archive");
                free(caminho_membro);
                free(caminho_target);
                exit(1);
            }

            opt_z(archive);

            fclose(archive);
            break;

        default:
            perror("Opção inválida\n");
            free(caminho_membro);
            free(caminho_target);
            exit(1);
    }

    free(caminho_membro);
    free(caminho_target);

    return 0;
}