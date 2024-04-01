canoml: main3.o 
	gcc -o canoml main3.o 

main3.o: main3.c
	gcc -c main3.c


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