
all: tema1

tema1: bmagic.o
	gcc -Wall -g -o tema1 bmagic.o

bmagic.o: bmagic.c
	gcc -Wall -g -c bmagic.c

run: tema1
	./tema1

clean:
	rm -f *.o tema1