CC:=gcc
CFLAGS:=-g -Wall -Wextra -o
PROGS:=A6-1 A6-2 A6-2x A6-3

.PHONY: all clean FORCE

all: $(PROGS)

$(PROGS): FORCE
	$(CC) $(CFLAGS) $@ $@.c

FORCE:

clean:
	rm -r $(PROGS) $(PROGS).dSYM