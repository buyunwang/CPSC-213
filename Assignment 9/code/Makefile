CC = /usr/local/bin/gcc-4.9
CFLAGS  += -std=gnu11 -Wall -g
UNAME = $(shell uname)
ifeq ($(UNAME), Linux)
LDFLAGS += -lpthread
endif
all: smoke washroom pc_sem smoke_sem washroom_sem
clean:
	rm -f *.o smoke washroom pc_sem smoke_sem washroom_sem; rm -rf *.dSYM
smoke: smoke.o uthread.o uthread_mutex_cond.o
washroom: washroom.o uthread.o uthread_mutex_cond.o
pc_sem: pc_sem.o uthread.o uthread_sem.o
smoke_sem: smoke_sem.o uthread.o uthread_sem.o
washroom_sem: washroom_sem.o uthread.o uthread_sem.o
