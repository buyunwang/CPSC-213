//
// Written by Mike Feeley, University of BC, 2010-2014
// Do not redistribute any portion of this code without permission.
//

#if __CYGWIN__
#ifndef NATIVE_STACK_SIZE
#define NATIVE_STACK_SIZE (1*1024*1024)
#endif
#ifndef PTHREAD_SUPPORT
#define PTHREAD_SUPPORT 1
#endif
#ifndef PTHREAD_SETSTACK_SUPPORT
#define PTHREAD_SETSTACK_SUPPORT 0
#endif

#else
#ifndef NATIVE_STACK_SIZE
#define NATIVE_STACK_SIZE (8*1024*1024)
#endif
#ifndef PTHREAD_SUPPORT
#define PTHREAD_SUPPORT  1
#endif
#ifndef PTHREAD_SETSTACK_SUPPORT
#define PTHREAD_SETSTACK_SUPPORT 1
#endif
#endif

#ifndef PTHREAD_IDLE_SLEEP
#define PTHREAD_IDLE_SLEEP 0
#endif
#ifndef SIG_PROTECTED
#define SIG_PROTECTED 1
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#if PTHREAD_SUPPORT
#include <pthread.h>
#endif
#if SIG_PROTECTED
#include <signal.h>
#endif
#include "spinlock.h"
#include "uthread.h"
#include "uthread_util.h"

#define TS_NASCENT 0
#define TS_RUNNING 1
#define TS_RUNABLE 8
#define TS_BLOCKED 3
#define TS_DYING   4
#define TS_DEAD    5

#define STACK_SIZE     (8*1024*1024)

#if SIG_PROTECTED
sigset_t uthread_protected_sigset;
#endif

#if SIG_PROTECTED
void uthread_setInterrupt (int isInterrupt);
int  uthread_isInterrupt  ();
int  init_complete = 0;
#endif

//
// SPINLOCKS
//

/**
 * spinlock_create
 */

void spinlock_create (spinlock_t* lock) {
  *lock = 0;
}

/**
 * spinlock_lock
 */

void spinlock_lock (spinlock_t* lock) {
#if SIG_PROTECTED
  if (!uthread_isInterrupt())
    sigprocmask (SIG_BLOCK, &uthread_protected_sigset, NULL);
#endif
  int already_held=1;
  do {
    while (*lock);
    asm volatile ("xchg  %0, %1\n" : "=m" (*lock), "=r" (already_held) : "1" (already_held));
  } while (already_held);
}

/**
 * spinlock_unlock
 */

void spinlock_unlock (spinlock_t* lock) {
  *lock = 0;
#if SIG_PROTECTED
  if (!uthread_isInterrupt())
    sigprocmask (SIG_UNBLOCK, &uthread_protected_sigset, NULL);
#endif
}

//
// THREAD CONTROL BLOCK
//

struct uthread_TCB {
  volatile int         state;                 
  volatile uintptr_t   saved_sp;              
  void*              (*start_proc) (void*);
  void*                start_arg;
  void*                return_val;
  void*                stack;
  spinlock_t           join_spinlock;
  uthread_t            joiner;
#if SIG_PROTECTED
  int                  isInterrupt;
#endif
  struct uthread_TCB*  next;
};

/**
 * uthread_initqueue
 */

void uthread_initqueue (uthread_queue_t* queue) {
  queue->head = 0;
  queue->tail = 0;
}

/**
 * uthread_enqueue
 */

void uthread_enqueue (uthread_queue_t* queue, uthread_t thread) {
  thread->next = 0;
  if (queue->tail)
    queue->tail->next = thread;
  queue->tail = thread;
  if (queue->head==0)
    queue->head = queue->tail;
}

/**
 * uthread_dequeue
 */

uthread_t uthread_dequeue (uthread_queue_t* queue) {
  uthread_t thread;
  if (queue->head) {    
    thread = queue->head;
    queue->head = queue->head->next;
    if (queue->head==0)
      queue->tail=0;
  } else 
    thread=0;
  return thread;
}

/**
 * uthread_queue_is_empty
 */

int uthread_queue_is_empty (uthread_queue_t* queue) {
  return queue->head == 0;
}

//
// READY QUEUE
//

static spinlock_t      ready_queue_spinlock;
static uthread_queue_t ready_queue;
#if PTHREAD_IDLE_SLEEP
pthread_mutex_t        pthread_mutex;
pthread_cond_t         pthread_wakeup;
int                    pthread_num_sleeping = 0;
pthread_key_t          pthread_base_thread;
#endif

/**
 * ready_queue_enqueue
 */

static void ready_queue_enqueue (uthread_t thread) {
  spinlock_lock (&ready_queue_spinlock);
  uthread_enqueue (&ready_queue, thread);
#if PTHREAD_IDLE_SLEEP
  if (pthread_num_sleeping) {
    pthread_mutex_lock   (&pthread_mutex);
    pthread_cond_signal  (&pthread_wakeup);
    pthread_mutex_unlock (&pthread_mutex);
  }
#endif
  spinlock_unlock (&ready_queue_spinlock);
}

/**
 * ready_queue_dequeue
 */

static uthread_t ready_queue_dequeue() {
  uthread_t thread = 0;
  
  while (! thread) {
    spinlock_lock (&ready_queue_spinlock);
    thread = uthread_dequeue (&ready_queue);
#if PTHREAD_IDLE_SLEEP
    if (! thread) {
      thread = (uthread_t) pthread_getspecific (pthread_base_thread);
      assert (thread);
      if (thread == uthread_self()) {
        // pthread_base is running ... stop the pthread
        pthread_mutex_lock   (&pthread_mutex);
        spinlock_unlock      (&ready_queue_spinlock);
        pthread_num_sleeping ++;
        pthread_cond_wait    (&pthread_wakeup, &pthread_mutex);
        pthread_num_sleeping --;
        pthread_mutex_unlock (&pthread_mutex);
        thread = 0;
        continue;
      }
    }
#endif
    spinlock_unlock (&ready_queue_spinlock);
  }
  return thread;
}

static void ready_queue_init () {
  spinlock_create   (&ready_queue_spinlock);
  uthread_initqueue (&ready_queue);
}

//
// UTHREAD PRIVATE IMPLEMENTATION
//

static void uthread_stop    (int);
static void uthread_start   (uthread_t);
static void uthread_free    (uthread_t);

//
// INITIALIZATION 
//

static uthread_t uthread_alloc      ();
static uthread_t uthread_new_thread (void* (*)(void*), void*);

static uthread_t base_thread;
static uintptr_t base_sp_lower_bound, base_sp_upper_bound;

#if PTHREAD_SETSTACK_SUPPORT==0
#define MAX_PTHREADS 100
static spinlock_t num_pthreads_spinlock = 0;
static int        num_pthreads = 0;
static uthread_t  pthread_base_threads[MAX_PTHREADS];
static uintptr_t  pthread_base_sp_lower_bound[MAX_PTHREADS], pthread_base_sp_upper_bound[MAX_PTHREADS];
#endif

static void* pthread_base (void* arg) {
#if PTHREAD_SETSTACK_SUPPORT==0
  if (arg) {
    spinlock_lock (&num_pthreads_spinlock);
    assert (num_pthreads < MAX_PTHREADS);
    pthread_base_threads        [num_pthreads] = (uthread_t) arg;
    pthread_base_sp_upper_bound [num_pthreads] = (((uintptr_t)&arg) + 1024);
    pthread_base_sp_lower_bound [num_pthreads] = (((uintptr_t)&arg) - NATIVE_STACK_SIZE);
    num_pthreads += 1;
    spinlock_unlock (&num_pthreads_spinlock);
  }
#endif
#if PTHREAD_IDLE_SLEEP
  pthread_setspecific (pthread_base_thread, uthread_self());
#endif
  while (1) {
#if PTHREAD_IDLE_SLEEP
    uthread_stop (TS_BLOCKED);
#else
    uthread_yield();
#endif
  }
  return NULL;
}

/**
 * uthread_init
 */

void uthread_init (int num_processors) {
  int dummy_local, i;
  uthread_t uthread;
#if PTHREAD_SUPPORT
  pthread_t pthread;
  pthread_attr_t attr;
#else
  assert (num_processors==1);
#endif
  
#if SIG_PROTECTED
  sigemptyset (& uthread_protected_sigset);
  sigaddset   (& uthread_protected_sigset, SIGALRM);
#endif
  base_sp_upper_bound = (((uintptr_t)&dummy_local) + 1024);
  base_sp_lower_bound = (((uintptr_t)&dummy_local) - NATIVE_STACK_SIZE);
  base_thread         = uthread_alloc ();
  base_thread->state  = TS_RUNNING;
  base_thread->stack  = 0;
  ready_queue_init      ();
#if PTHREAD_IDLE_SLEEP
  pthread_mutex_init           (&pthread_mutex, NULL);
  pthread_cond_init            (&pthread_wakeup, NULL);
  pthread_key_create           (&pthread_base_thread, 0);
  uthread = uthread_new_thread (pthread_base, 0);
  pthread_setspecific          (pthread_base_thread, uthread);
#else
  uthread = uthread_create     (pthread_base, 0);
#endif
#if PTHREAD_SUPPORT
  for (i=0; i<num_processors-1; i++) {
    uthread = uthread_new_thread (pthread_base, 0);
    uthread->state = TS_RUNNING;
    pthread_attr_init (&attr);
#if PTHREAD_SETSTACK_SUPPORT
    int err = pthread_attr_setstack (&attr, (void*) ((((uintptr_t) uthread->stack) + STACK_SIZE - 1) & ~(STACK_SIZE - 1)), STACK_SIZE);
    assert (! err);
#endif
    pthread_create (&pthread, &attr, pthread_base, uthread);
  }
#endif
#if SIG_PROTECTED
  init_complete = 1;
#endif
}

/**
 * uthread_alloc
 */

static uthread_t uthread_alloc () {
  uthread_t thread   = malloc (sizeof (struct uthread_TCB));
  thread->state      = TS_NASCENT;
  thread->start_proc = 0;
  thread->start_arg  = 0;
  thread->stack      = 0;
  thread->saved_sp   = 0;
  thread->joiner     = 0;
  spinlock_create (&thread->join_spinlock);
  return thread;
}

/**
 * uthread_new_thread
 */

static uthread_t uthread_new_thread (void* (*start_proc)(void*), void* start_arg) {
  uthread_t thread   = uthread_alloc ();
  thread->state      = TS_NASCENT;
  thread->start_proc = start_proc;
  thread->start_arg  = start_arg;
  thread->stack      = malloc (STACK_SIZE * 2);
  assert (thread->stack);
  thread->saved_sp   = ((((uintptr_t) thread->stack) + STACK_SIZE - 1) & ~(STACK_SIZE - 1)) + STACK_SIZE;
  *(uthread_t*) (thread->saved_sp -1 & ~(STACK_SIZE -1)) = thread;
  asm volatile (
#if __LP64__
// IA32-64
#define CLOBBERED_REGISTERS "%rax", "%rcx"
                "movq  %%rsp, %%rax\n"
                "movq %c1(%0), %%rsp\n"
                "subq $512, %%rsp\n"    // frame for uthread_switch
                "pushq $0\n"
                "pushq $0\n"
                "pushq $0\n"
                "pushq $0\n"
                "pushq $0\n"
                "pushq %%r8\n"
                "pushq %%r9\n"
                "pushq %%r10\n"
                "pushq %%r11\n"
                "pushq %%r12\n"
                "pushq %%r13\n"
                "pushq %%r14\n"
                "pushq %%r15\n"
                "pushfq\n"
                "movq  %%rsp, %%rcx\n"
                "addq  $256, %%rcx\n" // locate saved ebp within frame
                "pushq %%rcx\n"
                "movq %%rsp, %c1(%0)\n"
                "movq  %%rax, %%rsp\n"
#else
// IA32-32
#define CLOBBERED_REGISTERS "%eax", "%ecx"
                "movl  %%esp, %%eax\n"
                "movl  %c1(%0), %%esp\n"
                "subl  $512, %%esp\n"
                "pushl $0\n"
                "pushl $0\n"
                "pushl $0\n"
                "pushl $0\n"
                "pushl $0\n"
                "pushfl\n"
                "movl  %%esp, %%ecx\n"
                "addl  $256, %%ecx\n"
                "pushl %%ecx\n"
                "movl %%esp, %c1(%0)\n"
                "movl  %%eax, %%esp\n"
#endif
                : :
                "r" (thread),
                "i" (offsetof (struct uthread_TCB, saved_sp))
                : CLOBBERED_REGISTERS);
  return thread;
}

/**
 * uthread_switch
 */

static void uthread_switch (uthread_t to_thread, int from_thread_state) {
  uthread_t from_thread = uthread_self();
  
  asm volatile (
#if __LP64__
// IA32-64
                // save from_thread registers
                "pushq %%rbx\n"
                "pushq %%rcx\n"
                "pushq %%rdx\n"
                "pushq %%rsi\n"
                "pushq %%rdi\n"
                "pushq %%r8\n"
                "pushq %%r9\n"
                "pushq %%r10\n"
                "pushq %%r11\n"
                "pushq %%r12\n"
                "pushq %%r13\n"
                "pushq %%r14\n"
                "pushq %%r15\n"
                "pushfq\n"
                
                // save from_thread bp and sp
                "pushq %%rbp\n"
                "movq  %%rsp, %c6(%1)\n"
                
                // set from_thread_state
                "mfence\n"
                "movl  %3, %c5(%1)\n"
                
                // to_thread could still be running; wait for it
                "    movl  $%c4, %%eax\n"
                "L0: movl  %c5(%2), %%ebx\n"
                "    cmpl  %%eax, %%ebx\n"
                "    je    L0\n"
                
                // switch to to_thread
                "movq  %c6(%2), %%rsp\n"
                "popq  %%rbp\n"
                
                // set value of from_thread on new stack
                "movq  %1, %0\n"
                
                // restore to_thread
                "popfq\n"
                "popq  %%r15\n"
                "popq  %%r14\n"
                "popq  %%r13\n"            
                "popq  %%r12\n"
                "popq  %%r11\n"
                "popq  %%r10\n"
                "popq  %%r9\n"
                "popq  %%r8\n"
                "popq  %%rdi\n"
                "popq  %%rsi\n"
                "popq  %%rdx\n"
                "popq  %%rcx\n"
                "popq  %%rbx\n"
#else
// IA32-32
                // save from_thread registers
                "pushl %%ebx\n"
                "pushl %%ecx\n"
                "pushl %%edx\n"
                "pushl %%esi\n"
                "pushl %%edi\n"
                "pushfl\n"

                // save from_thread bp and sp
                "pushl %%ebp\n"
                "movl  %%esp, %c6(%1)\n"
                
                // set from_thread_state
                "mfence\n"
                "movl  %3, %c5(%1)\n"
                
                // to_thread could still be running; wait for it
                "    movl  $%c4, %eax\n"
                "L0: movl  %c5(%2), %%ebx\n"
                "    cmpl  %%eax, %ebx\n"
                "    je    L0\n"
                
                // switch to to_thread
                "movl  %c6(%2), %%esp\n"
                "popl  %%ebp\n"
                
                // set value of from_thread on new stack
                "movl  %1, %0\n"
                
                // restore to_thread
                "popfl\n"
                "popl  %%edi\n"
                "popl  %%esi\n"
                "popl  %%edx\n"
                "popl  %%ecx\n"
                "popl  %%ebx\n"
#endif
                :
                /* 0 */  "=m" (from_thread)
                :
                /* 1 */  "r" (from_thread),
                /* 2 */  "r" (to_thread),
                /* 3 */  "r" (from_thread_state),
                /* 4 */  "i" (TS_RUNNING),
                /* 5 */  "i" (offsetof (struct uthread_TCB, state)),
                /* 6 */  "i" (offsetof (struct uthread_TCB, saved_sp))
                : "%eax", "%ebx");
  
  if (from_thread->state == TS_DYING) {
    spinlock_lock (&from_thread->join_spinlock);
    if (from_thread->joiner == (uthread_t) -1)
      uthread_free (from_thread);
    else {
      from_thread->state = TS_DEAD;
      spinlock_unlock (&from_thread->join_spinlock);
      // at this point uthread_detach could free from_thread, so don't touch it after setting it to DEAD
    }
  }
  
  to_thread = uthread_self();
  if (to_thread->state == TS_NASCENT) {
    to_thread->state      = TS_RUNNING;
    to_thread->return_val = to_thread->start_proc (to_thread->start_arg);
    spinlock_lock (&to_thread->join_spinlock);
    to_thread->state = TS_DYING;
    if (to_thread->joiner != 0 && to_thread->joiner != (uthread_t) -1)
      uthread_start (to_thread->joiner);
    spinlock_unlock (&to_thread->join_spinlock);
    uthread_stop (TS_DYING);
  } else
    to_thread->state = TS_RUNNING;
}

/**
 * uthread_stop
 */

static void uthread_stop (int stopping_thread_state) {
  uthread_t to_thread = ready_queue_dequeue();
  assert (to_thread);
  uthread_switch (to_thread, stopping_thread_state);
}

/**
 * uthread_start
 *    Note that start does not set thread->state to TS_RUNNABLE because the thread might
 *    still be running.  Thread switch will wait for it to stop before re-starting it.
 *    But, as a consequence, it is possible to have a thread on the ready queue that is
 *    runnable but thread->state == TS_BLOCKED.  This is okay.
 */

static void uthread_start (uthread_t thread) {
  ready_queue_enqueue (thread);
}

/**
 * uthread_free
 */

static void uthread_free (uthread_t thread) {
  if (thread->stack)
    free (thread->stack);
  free (thread);
}


//
// UTHREAD PUBLIC INTERFACE
//

/**
 * uthread_create
 */

uthread_t uthread_create (void* (*start_proc)(void*), void* start_arg) {
  uthread_t thread = uthread_new_thread (start_proc, start_arg);
  ready_queue_enqueue (thread);
  return thread;
}

/**
 * uthread_self
 */

uthread_t uthread_self() {
  int dummy_local, i;

  if ((uintptr_t)&dummy_local >= base_sp_lower_bound && (uintptr_t) &dummy_local <= base_sp_upper_bound)
    return base_thread;
  else {
#if PTHREAD_SETSTACK_SUPPORT==0
    for (i=0; i<num_pthreads; i++)
      if ((uintptr_t)&dummy_local >= pthread_base_sp_lower_bound[i] && (uintptr_t) &dummy_local <= pthread_base_sp_upper_bound[i])
        return pthread_base_threads[i];
#else
    return *(uthread_t*) (void*) (((uintptr_t) &dummy_local) & ~(STACK_SIZE-1));
#endif
  }
}

/**
 * uthead_yield
 */

void uthread_yield() {
  ready_queue_enqueue (uthread_self());
  uthread_stop        (TS_RUNABLE);
}

/**
 * uthread_join
 */

int uthread_join (uthread_t thread, void** value_ptr) {
  if (thread->joiner == 0) {
    spinlock_lock (&thread->join_spinlock);
    if (thread->state != TS_DYING && thread->state != TS_DEAD) {
      thread->joiner = uthread_self();
      spinlock_unlock (&thread->join_spinlock);
      uthread_stop    (TS_BLOCKED);
      spinlock_lock   (&thread->join_spinlock);
    }
    if (value_ptr)
      *value_ptr = thread->return_val;
    if (thread->state == TS_DEAD)
      uthread_free (thread);
    else {
      thread->joiner = (uthread_t) -1;
      spinlock_unlock (&thread->join_spinlock);
    }
    return 0;
  } else 
    return -1;
}

/**
 * uthread_detach
 */

void uthread_detach (uthread_t thread) {
  if (thread->joiner == 0) {
    spinlock_lock (&thread->join_spinlock);
    if (thread->state != TS_DEAD) {
      thread->joiner = (uthread_t) -1;
      spinlock_unlock (&thread->join_spinlock);
    } else 
      uthread_free (thread);
  }
}

#if SIG_PROTECTED
/**
 * uthread_setInterrupt
 */

void uthread_setInterrupt (int isInterrupt) {
  if (init_complete)
    uthread_self() ->isInterrupt = isInterrupt;
}

/**
 * uthread_isInterrupt
 */

int uthread_isInterrupt () {
  return ! init_complete || uthread_self() ->isInterrupt;
}
#else
void uthread_setInterrupt (int isInterrupt) {}
#endif

/**
 * uthread_block
 */

void uthread_block () {
  uthread_stop (TS_BLOCKED);
}

/**
 * uthread_unblock
 */

void uthread_unblock (uthread_t thread) {
  uthread_start (thread);
}
