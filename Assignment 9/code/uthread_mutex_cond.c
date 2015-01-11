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
#include "uthread_mutex_cond.h"

//
// MONITORS (MUTEX) AND CONDITIONAL VARIABLES
//

struct uthread_mutex {
  uthread_t       holder;
  int             reader_count;
  spinlock_t      spinlock;
  uthread_queue_t waiter_queue;
  uthread_queue_t reader_waiter_queue;
};

struct uthread_cond {
  uthread_mutex_t mutex;
  uthread_queue_t waiter_queue;
};

/**
 * uthread_mutex_create
 */

uthread_mutex_t uthread_mutex_create () {
  uthread_mutex_t mutex = malloc (sizeof (struct uthread_mutex));
  mutex->holder = 0;
  mutex->reader_count = 0;
  spinlock_create   (&mutex->spinlock);
  uthread_initqueue (&mutex->waiter_queue);
  uthread_initqueue (&mutex->reader_waiter_queue);
  return mutex;
}

/**
 * uthread_mutex_destroy
 */

void uthread_mutex_destroy (uthread_mutex_t mutex) {
  free (mutex);
}

/**
 * uthread_mutex_lock
 */

void uthread_mutex_lock (uthread_mutex_t mutex) {
  spinlock_lock (&mutex->spinlock);
  while (mutex->holder || mutex->reader_count > 0) {
    uthread_enqueue (&mutex->waiter_queue, uthread_self());
    spinlock_unlock (&mutex->spinlock);
    uthread_block();
    spinlock_lock (&mutex->spinlock);
  }
  mutex->holder = uthread_self();
  spinlock_unlock (&mutex->spinlock);
}

/**
 * uthread_mutex_lock_readonly
 */

void uthread_mutex_lock_readonly (uthread_mutex_t mutex) {
  spinlock_lock (&mutex->spinlock);
  while (mutex->holder || !uthread_queue_is_empty (&mutex->waiter_queue)) {
    uthread_enqueue (&mutex->reader_waiter_queue, uthread_self());
    spinlock_unlock (&mutex->spinlock);
    uthread_block();
    spinlock_lock   (&mutex->spinlock);
  }
  mutex->reader_count += 1;
  spinlock_unlock (&mutex->spinlock);
}

/**
 * uthread_mutex_unlock
 */

void uthread_mutex_unlock (uthread_mutex_t mutex) {
  uthread_t waiter_thread;
  
  spinlock_lock (&mutex->spinlock);
  if (mutex->holder) {
    assert (mutex->holder == uthread_self());
    mutex->holder = 0;
  } else {
    assert (mutex->reader_count > 0);
    mutex->reader_count -= 1;
  }
  if (mutex->reader_count == 0) {
    waiter_thread = uthread_dequeue (&mutex->waiter_queue);
    if (waiter_thread)
      uthread_unblock (waiter_thread);
    else {
      while ((waiter_thread = uthread_dequeue (&mutex->reader_waiter_queue)))
        uthread_unblock (waiter_thread);
    }
  }
  spinlock_unlock (&mutex->spinlock);
}

/**
 * uthread_cond_create
 */

uthread_cond_t uthread_cond_create (uthread_mutex_t mutex) {
  uthread_cond_t cond = malloc (sizeof (struct uthread_cond));
  cond->mutex = mutex;
  uthread_initqueue (&cond->waiter_queue);
  return cond;
}

/**
 * uthread_cond_free
 */

void uthread_cond_destroy (uthread_cond_t cond) {
  free (cond);
}

/**
 * uthread_cond_wait
 */

void uthread_cond_wait (uthread_cond_t cond) {
  assert (cond->mutex->holder == uthread_self ());
  uthread_enqueue (&cond->waiter_queue, uthread_self());
  uthread_mutex_unlock (cond->mutex);
  uthread_block();
  uthread_mutex_lock (cond->mutex);
}

/**
 * uthread_cond_signal
 */

void uthread_cond_signal (uthread_cond_t cond) {
  uthread_t waiter_thread;
  
  assert (cond->mutex->holder == uthread_self ());
  waiter_thread = uthread_dequeue (&cond->waiter_queue);
  if (waiter_thread)
    uthread_unblock (waiter_thread);
}

/**
 * uthread_cond_broadcast
 */

void uthread_cond_broadcast (uthread_cond_t cond) {
  uthread_t waiter_thread;
  
  assert (cond->mutex->holder == uthread_self ());
  while ((waiter_thread = uthread_dequeue (&cond->waiter_queue)))
    uthread_unblock (waiter_thread);
}

