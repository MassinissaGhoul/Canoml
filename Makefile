canoml: main.o 
	gcc -o canoml main.o 

main.o: main.c
	gcc -c main.c


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