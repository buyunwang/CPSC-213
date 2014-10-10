CC=gcc
CFLAGS=-g -Wall -Wextra -O2
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
PROGS=BinaryTree

all: $(PROGS)
	
$@:
	$(CC) $(CFLAGS) $@ $@.c

.PHONY: clean
clean:
	rm $(PROGS)
	
