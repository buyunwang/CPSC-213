//
// Written by Mike Feeley, University of BC, 2010-2014
// Do not redistribute any portion of this code without permission.
//

#ifndef __uthread_sem_h__
#define __uthread_sem_h__

struct uthread_sem;
typedef struct uthread_sem* uthread_sem_t;

uthread_sem_t uthread_sem_create  (int initial_value);
void          uthread_sem_destroy (uthread_sem_t);
void          uthread_sem_wait    (uthread_sem_t);
void          uthread_sem_signal  (uthread_sem_t);

#endif

