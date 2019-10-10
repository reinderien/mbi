#!/usr/bin/make -f

export
# cflags=-Wall -std=c17 -ggdb
cflags=-Wall -std=c17 -march=native -O3

all: mbi
mbi: mbi.o
	gcc $$cflags -o $@ $<
mbi.o: mbi.c makefile
	gcc $$cflags -o $@ $< -c
debug: mbi
	gdb --args $< 1A00A00AA00 12

clean:
	rm -f mbi *.o

