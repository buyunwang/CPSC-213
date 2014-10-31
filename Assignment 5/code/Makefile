CC=gcc
CFLAGS=-Wall
SOURCES=A5-a.c A5-b.c
OBJECTS=$(SOURCES:.c=.o)
PROGS=$(SOURCES:.c=)

all: $(PROGS)

$@:
	$(CC) $(CFLAGS) $@ $@.c

clean:
	rm -rf $(OBJECTS) $(PROGS)
