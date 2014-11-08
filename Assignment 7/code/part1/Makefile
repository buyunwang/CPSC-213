CC:=gcc
CFLAGS:=-Wall -Wextra -O2 -o
PROGS:=A7-1 A7-2 A7-2-jumptable

.PHONY: all clean FORCE

all: $(PROGS)

$(PROGS): FORCE
	$(CC) $(CFLAGS) $@ $@.c

FORCE:

clean:
	rm -r $(PROGS) $(PROGS).dSYM