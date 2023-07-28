Trabalho produzido por Luan Marko Kujavski
GRR20221236

A ideia do programa vina++ é servir como arquivador. O pacote de software dá a possibilidade de armazenar uma sequência de arquivos em um archive.vpp

Para compilar o programa, deve ser usado o seguinte comando:
1.   make

Para limpar os arquivos gerados pelo programa, são válidos os seguintes comandos:
1.   make clean

Para executar o programa, são válidas as seguintes linhas de código:
1.   ./vina++ -i archive.vpp [membros]
2.   ./vina++ -a archive.vpp [membros]
3.   ./vina++ -m target archive.vpp [membros]
4.   ./vina++ -x archive.vpp [membros]
5.   ./vina++ -r archive.vpp [membros]
6.   ./vina++ -c archive.vpp [membros]
7.   ./vina++ -h
8.   ./vina++ -z archive.vpp
A opção -i adiciona membros ao archive
A opção -a adiciona membros ao archive e substitui membros passados caso a data de modificação do parâmetro passado seja mais recente
A opção -m muda a ordem em que as informações dos membros estão colocadas no diretório do archive
A opção -x descompacta os membros passados. Caso nenhum membro seja passado, tudo é descompactado
A opção -r remove os membros passados do archive
A opção -c lista os membros que estão atualmente no archive
A opção -h lista as opções que estão disponíveis no programa
A opção -z limpa todo o archive
Vale mencionar que as operações usam sempre caminhos relativos para arquivar. Assim, caso você volte a fazer operações com algum arquivo passado, você deverá escrever o caminho exatamente como foi passado na primeira vez

O pacote de software é composto em um arquivo principal (vina++.c) e três bibliotecas (archive.h, lista.h e opt.h)
Basicamente, archive.h guarda as funções relativas ao archive. A biblioteca lista.h guarda funções que operam uma lista encadeada. A biblioteca opt.h guarda funções relativas às operações que são aplicadas no software, uma junção das outras bibliotecas

No pacote, existe um diretório chamado "src", onde estão localizados o programa e as bibliotecas. Existe também um makefile e o LEIAME.txt

Para a correta execução do programa, foi utilizada uma lista encadeada que opera o diretório do archive. São guardadas todas as informações dos membros passados nela. Foi utilizada essa estrutura de dados pela facilidade de manipulação de informações nela e pela alocação dinâmica de memória para que vários membros possam ser colocados no archive
Também, o limite de leitura de dados do programa é 1024 bytes. Assim, o conteúdo do archive é manipulado a partir desse limite

O único BUG conhecido é tentar descompactar um membro cujo caminho passe por diretórios anteriores (como ".."). Essa opção funciona apenas para criar arquivos em diretórios que vêm depois e no atual diretório
