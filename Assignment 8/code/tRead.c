#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "queue.h"
#include "disk.h"
#include "uthread.h"

queue_t waitq;

void interruptServiceRoutine () {
  uthread_unblock(queue_dequeue (&waitq));
}

void blockUntilComplete() {
  uthread_block(NULL, NULL);
}

struct ReadArgs {
  char* buf;
  int   nbytes;
  int   blockno;
};

void handleRead (char* buf, int nbytes, int blockno) {
  assert (*((int*)buf) == blockno);
}

void* readAndHandleBlock (void* argsv) {
  struct ReadArgs * args = argsv;
  queue_enqueue(&waitq, uthread_self());
  disk_scheduleRead (args->buf, args->nbytes, args->blockno);
  blockUntilComplete();
  handleRead (args->buf, args->nbytes, args->blockno);
  return NULL;
}

void run (int numBlocks) {
  uthread_t thread[numBlocks];
  struct ReadArgs args[numBlocks];
  char buf[numBlocks][8];

  for (int blockno = 0; blockno < numBlocks; blockno++) {
    args[blockno].buf = buf[blockno];
    args[blockno].nbytes = 8;
    args[blockno].blockno = blockno;

    thread[blockno] = uthread_create(readAndHandleBlock, &args[blockno]);
  }
  for (int i=0; i<numBlocks; i++)
    uthread_join (thread[i], 0);
}

int main (int argc, char** argv) {
  static const char* usage = "usage: tRead numBlocks";
  int numBlocks = 0;

  if (argc == 2)
    numBlocks = strtol (argv [1], NULL, 10);
  if (argc != 2 || (numBlocks == 0 && errno == EINVAL)) {
    printf ("%s\n", usage);
    return EXIT_FAILURE;
  }

  uthread_init (1);
  disk_start   (interruptServiceRoutine);

  queue_init (&waitq);
  run (numBlocks);
}
