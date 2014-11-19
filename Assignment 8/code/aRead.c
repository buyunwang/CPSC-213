//
// THIS IS THE SOLUTION TO ASSIGNMENT 7.  DO NOT DISTRIBUTE.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "uthread.h"
#include "queue.h"
#include "disk.h"

struct PendingRead {
  char* buf;
  int   nbytes;
  int   blockno;
  void  (*handler) (char*, int, int);
};

queue_t prq;

void interruptServiceRoutine () {
  struct PendingRead* pr = queue_dequeue (&prq);
  pr->handler (pr->buf, pr->nbytes, pr->blockno);
}

void asyncRead (char* buf, int nbytes, int blockno, void (*handler) (char*, int, int)) {
  struct PendingRead* pr = malloc (sizeof (struct PendingRead));
  pr->buf     = buf;
  pr->nbytes  = nbytes;
  pr->blockno = blockno;
  pr->handler = handler;
  queue_enqueue (&prq, pr);
  disk_scheduleRead (buf, nbytes, blockno);
}

void handleRead (char* buf, int nbytes, int blockno) {
  assert (*((int*)buf) == blockno);
}

void run (int numBlocks) {
  char* buf = malloc (numBlocks * 8);
  for (int blockno = 0; blockno < numBlocks; blockno++) {
    asyncRead (buf + blockno, 8, blockno, handleRead);
  }
  disk_waitForReads();
  free (buf);
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
  queue_init   (&prq);
  
  run (numBlocks);
}