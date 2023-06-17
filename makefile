CC=gcc
CFLAGS=-g -ansi -Wall -pedantic

all: mycomp

mycomp: mycomp.o complex.o
	$(CC) $(CFLAGS) -o mycomp mycomp.o complex.o -lm

mycomp.o: mycomp.c complex.h
	$(CC) $(CFLAGS) -c mycomp.c

complex.o: complex.c complex.h
	$(CC) $(CFLAGS) -c complex.c

clean:
	rm -f mycomp *.o