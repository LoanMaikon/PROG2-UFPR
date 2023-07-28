CFLAGS = -Wall -std=c99 -g
LDFLAGS = -lm
CC = gcc
objects = beale.o arquivo_chaves.o codificacao.o decodificacao.o lista.o
     
beale: beale.o arquivo_chaves.o codificacao.o decodificacao.o lista.o
	$(CC) -o beale beale.o arquivo_chaves.o codificacao.o decodificacao.o lista.o $(LDFLAGS)

beale.o: beale.c
	$(CC) -c $(CFLAGS) beale.c

lista.o: lista.c
	$(CC) -c $(CFLAGS) lista.c

arquivo_chaves.o: arquivo_chaves.c
	$(CC) -c $(CFLAGS) arquivo_chaves.c

codificacao.o: codificacao.c
	$(CC) -c $(CFLAGS) codificacao.c

decodificacao.o: decodificacao.c
	$(CC) -c $(CFLAGS) decodificacao.c

clean:
	rm -f $(objects)

purge:
	rm -f $(objects) beale
