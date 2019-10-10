#!/usr/bin/make -f

export
cflags=-Wall -ggdb -std=c17

all: mbi
mbi: mbi.o
	gcc $$cflags -o $@ $^
mbi.o: mbi.c
	gcc $$cflags -o $@ $^ -c

clean:
	rm -f mbi *.o

