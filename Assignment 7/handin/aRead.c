#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "uthread.h"
#include "queue.h"
#include "disk.h"

queue_t prq;

struct PendingRead {
  char* buf;
  int nbytes;
  int blockno;
  void (*handler) (char*, int, int);
};

void interruptServiceRoutine () {
  struct PendingRead* pendingRead = queue_dequeue(&prq);
  pendingRead->handler(pendingRead->buf, pendingRead->nbytes, pendingRead->blockno);
}

void asyncRead (char* buf, int nbytes, int blockno, void (*handler) (char*, int, int)) {
  struct PendingRead* pendingRead = malloc(sizeof(struct PendingRead));
  pendingRead->buf = buf;
  pendingRead->nbytes = nbytes;
  pendingRead->blockno = blockno;
  pendingRead->handler = handler;
  queue_enqueue(&prq, pendingRead);
  disk_scheduleRead(buf, nbytes, blockno);
}

void handleRead (char* buf, int nbytes, int blockno) {
  assert (*((int*)buf) == blockno);
}

void run (int numBlocks) {
  queue_init(&prq);
  char buf[8];
  for (int blockno = 0; blockno < numBlocks; blockno++) {
    asyncRead (buf, 8, blockno, handleRead);
  }
  disk_waitForReads();
}

int main (int argc, char** argv) {
  static const char* usage = "usage: aRead numBlocks";
  int numBlocks = 0;

  if (argc == 2)
    numBlocks = strtol (argv [1], NULL, 10);
  if (argc != 2 || (numBlocks == 0 && errno == EINVAL)) {
    printf ("%s\n", usage);
    return EXIT_FAILURE;
  }
  
  uthread_init (1);
  disk_start   (interruptServiceRoutine);
  
  run (numBlocks);
}
