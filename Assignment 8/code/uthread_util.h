//
// Written by Mike Feeley, University of BC, 2010-2014
// Do not redistribute any portion of this code without permission.
//

#ifndef __uthread_util_h__
#define __uthread_util_h__

struct uthread_queue {
  uthread_t head, tail;
};
typedef struct uthread_queue uthread_queue_t;

void      uthread_initqueue      (uthread_queue_t*);
void      uthread_enqueue        (uthread_queue_t*, uthread_t);
uthread_t uthread_dequeue        (uthread_queue_t*);
int       uthread_queue_is_empty (uthread_queue_t* queue);

void uthread_setInterrupt (int);

#endif
