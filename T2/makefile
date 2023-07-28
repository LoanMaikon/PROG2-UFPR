CFLAGS = -Wall -std=c99 -D_XOPEN_SOURCE=500
LDFLAGS = -lm
CC = gcc

vina++: src/vina++.o src/lista.o src/archive.o src/opt.o
	$(CC) -o vina++ src/vina++.o src/lista.o src/archive.o src/opt.o $(LDFLAGS)

src/vina++.o: src/vina++.c
	$(CC) -c $(CFLAGS) src/vina++.c -o src/vina++.o

src/lista.o: src/lista.c
	$(CC) -c $(CFLAGS) src/lista.c -o src/lista.o

src/archive.o: src/archive.c
	$(CC) -c $(CFLAGS) src/archive.c -o src/archive.o

src/opt.o: src/opt.c
	$(CC) -c $(CFLAGS) src/opt.c -o src/opt.o

clean:
	rm -f src/vina++.o vina++ src/lista.o src/archive.o src/opt.o