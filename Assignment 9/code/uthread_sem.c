//
// Written by Mike Feeley, University of BC, 2010-2014
// Do not redistribute any portion of this code without permission.
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "spinlock.h"
#include "uthread.h"
#include "uthread_util.h"
#include "uthread_sem.h"

//
// SEMAPHORES
//

struct uthread_sem {
  int             value;
  spinlock_t      spinlock;
  uthread_queue_t waiter_queue;
};

/**
 * uthread_sem_create
 */

uthread_sem_t uthread_sem_create (int initial_value) {
  uthread_sem_t sem = malloc (sizeof (struct uthread_sem));
  
  sem->value = initial_value;
  spinlock_create   (&sem->spinlock);
  uthread_initqueue (&sem->waiter_queue);
  return sem;
}

/**
 * uthread_sem_destroy
 */

void uthread_sem_destroy (uthread_sem_t sem) {
  free (sem);
}

/**
 * uthread_sem_signal (aka increment or V)
 */

void uthread_sem_signal (uthread_sem_t sem) {
  uthread_t waiter_thread;
  
  spinlock_lock (&sem->spinlock);
  sem->value += 1;
  waiter_thread = uthread_dequeue (&sem->waiter_queue);
  if (waiter_thread)
    uthread_unblock (waiter_thread);
  spinlock_unlock (&sem->spinlock);
}

/**
 * uthread_sem_wait (aka decrement or P)
 */

void uthread_sem_wait (uthread_sem_t sem) {
  uthread_t waiter_thread;
  
  spinlock_lock (&sem->spinlock);
  while (sem->value < 1) {
    uthread_enqueue (&sem->waiter_queue, uthread_self());
    spinlock_unlock (&sem->spinlock);
    uthread_block();
    spinlock_lock (&sem->spinlock);
  }
  sem->value -= 1;
  spinlock_unlock (&sem->spinlock);
}

