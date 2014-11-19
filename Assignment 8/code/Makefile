CC = /usr/local/bin/gcc-4.9
CFLAGS  += -std=gnu11 -Wall -g
UNAME = $(shell uname)
ifeq ($(UNAME), Linux)
LDFLAGS += -lpthread
endif
all: aRead sRead tRead pc_spinlock pc_mutex_cond
clean:
	rm -f *.o aRead sRead tRead pc_spinlock pc_mutex_cond; rm -rf *.dSYM
aRead: aRead.o disk.o queue.o uthread.o
sRead: sRead.o disk.o uthread.o
tRead: tRead.o disk.o uthread.o queue.o
pc_spinlock:  pc_spinlock.o uthread.o
pc_mutex_cond:  pc_mutex_cond.o uthread.o uthread_mutex_cond.o
