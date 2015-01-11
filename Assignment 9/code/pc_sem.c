#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uthread.h"
#include "uthread_sem.h"

#define MAX_ITEMS 10
#define NUM_ITERATIONS 25000
#define NUM_CONSUMERS 2
#define NUM_PRODUCERS 2

static int items = 0;
static int histogram [MAX_ITEMS + 1];

static uthread_sem_t lock;
static uthread_sem_t count_from_empty;
static uthread_sem_t count_from_full;

void produce() {
  uthread_sem_wait(count_from_full);
  uthread_sem_wait(lock);
  items++;
  histogram[items]++;
  assert(items <= MAX_ITEMS);
  uthread_sem_signal(lock);
  uthread_sem_signal(count_from_empty);
}

void consume() {
  uthread_sem_wait(count_from_empty);
  uthread_sem_wait(lock);
  items--;
  histogram[items]++;
  assert (items >= 0);
  uthread_sem_signal(lock);
  uthread_sem_signal(count_from_full);
}

void* producer() {
  for (int i=0; i < NUM_ITERATIONS; i++)
    produce();
  return NULL;
}

void* consumer() {
  for (int i=0; i < NUM_ITERATIONS; i++)
    consume();
  return NULL;
}

int main (int argc, char** argv) {
  assert (NUM_PRODUCERS == NUM_CONSUMERS);

  lock = uthread_sem_create(1);
  count_from_empty = uthread_sem_create(0);
  count_from_full = uthread_sem_create(MAX_ITEMS);

  uthread_init(NUM_PRODUCERS + NUM_CONSUMERS);
  uthread_t producers[NUM_PRODUCERS];
  uthread_t consumers[NUM_CONSUMERS];

  for (int i = 0; i < NUM_PRODUCERS; i++) {
    consumers[i] = uthread_create(consumer, NULL);
    producers[i] = uthread_create(producer, NULL);
  }

  for (int i = 0; i < NUM_PRODUCERS; i++) {
    uthread_join(consumers[i], NULL);
    uthread_join(producers[i], NULL);
  }

  printf("Histogram: [ ");
  int sum = 0;
  for (int i = 0; i < MAX_ITEMS + 1; i++) {
    sum += histogram[i];
    printf("%d ", histogram[i]);
  }
  printf("]\nSum: %d\n", sum);
}
