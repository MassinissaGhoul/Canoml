canoml: main2.o 
	gcc -o canoml main2.o 

main2.o: main2.c
	gcc -c main2.c


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