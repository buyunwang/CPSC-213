CFLAGS  += -std=gnu11
UNAME = $(shell uname)
ifeq ($(UNAME), Linux)
LDFLAGS += -pthread
endif
all: aRead sRead 
clean:
	rm -f *.o aRead sRead; rm -rf *.dSYM
aRead: aRead.o disk.o queue.o uthread.o
sRead: sRead.o disk.o uthread.o
