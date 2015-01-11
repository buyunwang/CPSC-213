//
// Written by Mike Feeley, University of BC, 2010-2014
// Do not redistribute any portion of this code without permission.
//

#ifndef __spinlock_h__
#define __spinlock_h__

typedef volatile int spinlock_t;
void       spinlock_create (spinlock_t* lock);
void       spinlock_lock   (spinlock_t* lock);
void       spinlock_unlock (spinlock_t* lock);

#endif