# Numele executabilului va fi temaX, unde X este numărul temei
EXECUTABLE = quadtree

# Lista fișierelor sursă
SOURCE_FILES = main.c

# Opțiuni de compilare

.PHONY: all build run clean

# Regula implicită
all: build

# Regula pentru compilare
build:
	gcc  main.c  -o $(EXECUTABLE)

# Regula pentru rulare
run:
	./$(EXECUTABLE)

# Regula pentru ștergerea executabilului
clean:
	rm -f $(EXECUTABLE)