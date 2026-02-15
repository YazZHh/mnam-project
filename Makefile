CC = gcc -g -Wall 

all: bin/ bin/main

bin/:
	mkdir -p bin/

bin/main: bin/main.o bin/grille.o bin/drone.o
	$(CC) -o $@ $^

bin/main.o: src/main.c
	$(CC) -c -o $@ $<

bin/grille.o: src/grille.c
	$(CC) -c -o $@ $<

bin/drone.o: src/drone.c
	$(CC) -c -o $@ $<

clean:
	rm -rf bin/*