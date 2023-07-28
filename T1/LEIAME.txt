Luan Marko Kujavski
GRR20221236

O programa consiste em um programa principal beale.c e outras quatro bibliotecas: arquivo_chaves.c, codificacao.c, decodificacao.c e lista.c com seus respectivos arquivos de cabeçalho (.h).
O arquivo lista.c contém as funções para criar a lista encadeada, excluir a lista e adicionar um caracter com seu código na lista.
O arquivo arquivo_chaves.c contém as funções para gerar a lista a partir do arquivo de chaves ou a partir do livro cifra e escrever o conteúdo da lista no arquivo de chaves.
Os arquivos codificacao.c e decodificacao.c contêm as funções para, respectivamente, imprimir os códigos no arquivo da mensagem codificada e imprimir os caracteres no arquivo da mensagem decodificada.

A estrutura de dados usada foi a lista, descrita mais detalhadamente na lista.h. Basicamente, existe a struct cabeca_nodo_t que aponta para o primeiro nodo da lista. Cada nodo, declarado como nodo_lista_t, tem um caracter, um código e um ponteiro apontando para o próximo nodo.
Para essa estrutura de dados, fiz as funções cria_lista, que aloca espaço para a lista, destroi_lista, que desaloca o espaço da lista, e adiciona_ordem_lista, que adiciona um caracter e seu código seguindo a ordem ASCII.

Para compilar o programa, deve ser usado o seguinte comando:
1.   make

Para limpar os arquivos gerados pelo programa, são válidos os seguintes comandos:
1.   make clean
2.   make purge

Para executar o programa, são válidas as seguintes linhas de código:
1.   ./beale  -e  -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves 
2.   ./beale  -d  -i MensagemCodificada  -c ArquivoDeChaves  -o MensagemDecodificada
3.   ./beale -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada 

O primeiro comando recebe um livro cifra e, a partir dele, gera um arquivo de chaves para codificar a mensagem original e escrever na mensagem codificada.
O segundo comando recebe uma mensagem codificada e, a partir do arquivo de chaves, decodifica ela.
O terceiro comando recebe uma mensagem codificada e, a partir do livro cifra, decodifica ela.

O funcionamento do programa é codificar ou decodificar, a cada execução, mensagens que você passa na linha de comando. Usando a ideia da Cifra de Baele, na qual pegamos o primeiro caracter de cada palavra e geramos um código para ele, codificamos ou decodificamos qualquer mensagem.

ADENDOS
A codificação de alguns caracteres seguiu algumas regras:
1.   O espaço é codificada e decodificado pela chave -1
2.   Caracteres desconhecidos são codificados pela chave -2 e decodificados pelo caracter "?"
3.   O salto de linha (\n) é codificada e decodificado pela chave -3
