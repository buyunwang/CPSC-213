//
// Written by Mike Feeley, University of BC, 2010-2014
// Do not redistribute any portion of this code without permission.
//

#ifndef __uthread_mutex_cond_h__
#define __uthread_mutex_cond_h__

struct uthread_mutex;
typedef struct uthread_mutex* uthread_mutex_t;
struct uthread_cond;
typedef struct uthread_cond*  uthread_cond_t;

uthread_mutex_t uthread_mutex_create        ();
void            uthread_mutex_lock          (uthread_mutex_t);
void            uthread_mutex_lock_readonly (uthread_mutex_t);
void            uthread_mutex_unlock        (uthread_mutex_t);
void            uthread_mutex_destroy       (uthread_mutex_t);

uthread_cond_t  uthread_cond_create         (uthread_mutex_t);
void            uthread_cond_wait           (uthread_cond_t);
void            uthread_cond_signal         (uthread_cond_t);
void            uthread_cond_broadcast      (uthread_cond_t);
void            uthread_cond_destroy        (uthread_cond_t);

#endif
