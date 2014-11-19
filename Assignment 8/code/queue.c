#include <stdlib.h>
#include <signal.h>
#include "uthread.h"
#include "queue.h"

struct queue_e {
  void*           data;
  struct queue_e* prev;
  struct queue_e* next;
};

void queue_init (queue_t* q) {
  q->front = NULL;
  q->back  = NULL;
  q->free  = NULL;
  spinlock_create (&q->mutex);
}

void queue_enqueue (queue_t* q, void* data) {
  spinlock_lock (&q->mutex);
    struct queue_e* qe;
    if (q->free != NULL) {
      qe      = q->free;
      q->free = q->free->next;
    } else
      qe = malloc (sizeof (struct queue_e));
    qe->data = data;
    qe->prev = NULL;
    qe->next = q->back;
    if (q->back != NULL)
      q->back->prev = qe;
    else
      q->front = qe;
    q->back = qe;
  spinlock_unlock (&q->mutex);
}

void* queue_dequeue (queue_t* q) {
  void* data = NULL;
  spinlock_lock (&q->mutex);
    if (q->front != NULL) {
      data = q->front->data;
      struct queue_e* new_front = q->front->prev;
      q->front->next = q->free;
      q->free        = q->front;
      if (new_front != NULL)
        new_front->next = NULL;
      else
        q->back = NULL;
      q->front = new_front;
    }
  spinlock_unlock (&q->mutex);
  return data;
}