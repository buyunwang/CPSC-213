#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "uthread.h"
#include "disk.h"

void interruptServiceRoutine () {
}

void syncRead (char* buf, int nBytes, int blockNo) {
  disk_scheduleRead (buf, 4096, blockNo);
  disk_waitForReads ();
  assert (*((int*) buf) == blockNo);
}

void handleRead (char* buf, int nbytes, int blockno) {
  assert (*((int*)buf) == blockno);
}

void run (int numBlocks) {
  char buf [8];
  for (int blockno = 0; blockno < numBlocks; blockno++) {
    syncRead   (buf, sizeof (buf), blockno);
    handleRead (buf, sizeof (buf), blockno);
  }
}

int main (int argc, char** argv) {
  static const char* usage = "usage: sRead numBlocks";
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