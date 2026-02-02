CC = gcc -g -Wall -Werror

all: bin/main

bin/main: bin/main.o bin/grille.o
	$(CC) -o $@ $^

bin/main.o: src/main.c
	$(CC) -c -o $@ $<

bin/grille.o: src/grille.c
	$(CC) -c -o $@ $<

clean:
	rm -rf bin/*