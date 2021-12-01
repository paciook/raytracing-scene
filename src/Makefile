CFLAGS=-Wall -Werror -std=c99 -pedantic -g
LDLIBS=-lm
all: main

main: main.o vector.o luz.o color.o arreglo.o objetos.o
	gcc $(CFLAGS) *.o $(LDLIBS) -o main

main.o: main.c color.h luz.h arreglo.h vector.h objetos.h imagen.h
	gcc $(CFLAGS) -c main.c

vector.o: vector.c vector.h
	gcc $(CFLAGS) -c vector.c

luz.o: luz.c luz.h color.h vector.h
	gcc $(CFLAGS) -c luz.c

imagen.o: imagen.c imagen.h
	gcc $(CFLAGS) -c imagen.c

objetos.o: objetos.c objetos.h
	gcc $(CFLAGS) -c objetos.c

color.o: color.h	
	gcc $(CFLAGS) -c color.c

arreglo.o: arreglo.h
	gcc $(CFLAGS) -c arreglo.c

clean:
	rm -vf *.o main
