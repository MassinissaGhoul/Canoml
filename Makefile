opt = -c -Wall -Wextra

canoml: main.o arbre.o analyseur.o
	gcc -o canoml main.o arbre.o analyseur.o

main.o: main.c
	gcc ${opt} main.c

arbre.o: arbre.c
	gcc ${opt} arbre.c

analyseur.o: analyseur.c
	gcc ${opt} analyseur.c


ifneq ($(OS),Windows_NT)
clean:
		rm -f *.o
else
clean:
		del /f /q *.o
endif

all:
		make canoml
		make clean