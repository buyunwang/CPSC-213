#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/errno.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include "uthread.h"
#include "uthread_util.h"
#include "disk.h"


#define READ_LATENCY_USEC (10*1000)

const int TIMER_SIGNO = SIGALRM;
sigset_t  TIMER_SIGSET;

spinlock_t prq_mutex = 0;

void (*isr) (void);

void tm_add (struct timeval* tm, int usec) {
  tm->tv_usec += usec;
  if (tm->tv_usec > 1000*1000) {
    tm->tv_sec  += 1;
    tm->tv_usec -= 1000*1000;
  } else if (tm->tv_usec < 0) {
    tm->tv_sec  -= 1;
    tm->tv_usec += 1000*1000;
  }
}

int tm_compare (struct timeval* a, struct timeval* b) {
  if (a->tv_sec < b->tv_sec)
    return -1;
  else if (a->tv_sec > b->tv_sec)
    return 1;
  else if (a->tv_usec < b->tv_usec)
    return -1;
  else if (a->tv_usec > b->tv_usec)
    return 1;
  else
    return 0;
}

struct PendingRead {
  char*               buf;
  int                 blockNo;
  struct timeval      completeTime;
  struct PendingRead* prev,
                    * next;
} *prq_front = NULL,
  *prq_back  = NULL,
  *prq_free  = NULL;

void prq_enqueue (char* buf, int blockNo) {
  spinlock_lock (&prq_mutex);
    struct PendingRead* pr;
    if (prq_free != NULL) {
      pr       = prq_free;
      prq_free = prq_free->next;
    } else
      pr = malloc (sizeof (struct PendingRead));
    pr->buf      = buf;
    pr->blockNo  = blockNo;
    gettimeofday (& pr ->completeTime, NULL);
    tm_add       (& pr ->completeTime, READ_LATENCY_USEC);
    pr->prev     = NULL;
    pr->next     = prq_back;
    if (prq_back == NULL)
      prq_back = (prq_front = pr);
    else {
      prq_back->prev = pr;
      prq_back       = pr;
    }
  spinlock_unlock (&prq_mutex);
}

void prq_dequeue_lock_held () {
  if (prq_front != NULL) {
    struct PendingRead* pr = prq_front;
    prq_front = pr->prev;
    if (prq_front == NULL)
      prq_back = NULL;
    pr->next = prq_free;
    prq_free = pr;
  }
}

void disk_scheduleRead (char* buf, int nBytes, int blockNo) {
  prq_enqueue (buf, blockNo);
}

void disk_waitForReads () {
  int done = 0;
  while (1) {
    while (prq_front != NULL) {}
    spinlock_lock (&prq_mutex);
      done = (prq_front == NULL);
    spinlock_unlock (&prq_mutex);
    if (done)
      return;
  }
}

void performDMA (char* buf, int blockno) {
  *((int*) buf) = blockno;
}

void deliverInterrupt() {
  uthread_setInterrupt (1);
  isr();
  uthread_setInterrupt (0);
}

void handleTimerInterrupt (int signo, siginfo_t* info, void* uap) {
  struct timeval now;
  gettimeofday (&now, NULL);
  
  spinlock_lock (&prq_mutex);
    while (prq_front != NULL && tm_compare (&prq_front->completeTime, &now) <= 0) {
      performDMA       (prq_front->buf, prq_front->blockNo);
      deliverInterrupt ();
      prq_dequeue_lock_held();
    }
  spinlock_unlock (&prq_mutex);
}

void disk_start (void (*interruptServiceRoutine) ()) {
  isr = interruptServiceRoutine;
  spinlock_create (&prq_mutex);
  sigemptyset (& TIMER_SIGSET);
  sigaddset   (& TIMER_SIGSET, TIMER_SIGNO);
  struct sigaction sa;
  sa .sa_sigaction = handleTimerInterrupt;
  sigemptyset (& sa .sa_mask);
  sa .sa_flags     = SA_SIGINFO;
  int ok = sigaction (TIMER_SIGNO, &sa, NULL);
  if (ok == -1) {
    printf ("DISK sigaction: %s\n", strerror (errno));
    exit (EXIT_FAILURE);
  }
  ualarm (READ_LATENCY_USEC / 10, READ_LATENCY_USEC / 10);
}

