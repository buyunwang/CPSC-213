CC=gcc
CFLAGS=-g -Wall
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.cpp=.o)
PROGS=dangling-pointers

all: $(PROGS)
	
$@:
	$(CC) $(CFLAGS) $@ $@.c

clean:
	rm -rf *o
