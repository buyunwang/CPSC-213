CC = /usr/local/bin/gcc-4.9
CFLAGS  += -std=gnu11 -Wall -g
UNAME = $(shell uname)
ifeq ($(UNAME), Linux)
LDFLAGS += -lpthread
endif
all: smoke
clean:
	rm -f *.o smoke; rm -rf *.dSYM
smoke: smoke.o uthread.o uthread_mutex_cond.o
