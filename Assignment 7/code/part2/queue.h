#ifndef __queue_h__
#define __queue_h__

typedef volatile int spinlock_t;

struct queue_s {
  struct queue_e* front;
  struct queue_e* back;
  struct queue_e* free;
  spinlock_t      mutex;
};
typedef struct queue_s queue_t;

void  queue_init    (queue_t* q);
void  queue_enqueue (queue_t* q, void* data);
void* queue_dequeue (queue_t* q);

#endif