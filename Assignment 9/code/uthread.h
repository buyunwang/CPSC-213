//
// Written by Mike Feeley, University of BC, 2010-2014
// Do not redistribute any portion of this code without permission.
//

#ifndef __uthread_h__
#define __uthread_h__

struct uthread_TCB;
typedef struct uthread_TCB* uthread_t;

void      uthread_init    (int num_processors);
uthread_t uthread_create  (void* (*start_proc)(void*), void* start_arg);
void      uthread_detach  (uthread_t thread);
int       uthread_join    (uthread_t thread, void** value_ptr);
uthread_t uthread_self();
void      uthread_yield();
void      uthread_block();
void      uthread_unblock (uthread_t thread);

#endif