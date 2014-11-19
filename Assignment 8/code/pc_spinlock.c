#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uthread.h"

#define MAX_ITEMS 10
#define NUM_ITERATIONS 250000
#define NUM_CONSUMERS 2
#define NUM_PRODUCERS 2

static int items = 0;
static int producer_wait_count = 0;
static int consumer_wait_count = 0;
static int histogram [MAX_ITEMS + 1];

spinlock_t item_lock;
spinlock_t producer_wait_count_lock;
spinlock_t consumer_wait_count_lock;

void atomic_inc(int* val, spinlock_t* lock) {
  spinlock_lock(lock);
  (*val)++;
  spinlock_unlock(lock);
}

void produce() {
  for (;;) {
    // spin
    while (items >= MAX_ITEMS) {
      atomic_inc(&producer_wait_count, &producer_wait_count_lock);
    }
    // lock and test
    spinlock_lock(&item_lock);
    if (items >= MAX_ITEMS) {     // condition no longer true
      atomic_inc(&producer_wait_count, &producer_wait_count_lock);
      spinlock_unlock(&item_lock);
    }
    else {                        // lock successful
      break;
    }
  }
  // update
  items++;
  histogram[items]++;
  assert (items <= MAX_ITEMS);
  // unlock
  spinlock_unlock(&item_lock);
}

void consume() {
  for (;;) {
    // spin
    while (items <= 0) {
      atomic_inc(&consumer_wait_count, &consumer_wait_count_lock);
    }
    // lock and test
    spinlock_lock(&item_lock);
    if (items <= 0) {             // condition no longer true
      atomic_inc(&consumer_wait_count, &consumer_wait_count_lock);
      spinlock_unlock(&item_lock);
    }
    else {
      break;                      // lock successful
    }
  }
  // update
  items--;
  histogram[items]++;
  assert (items >= 0);
  // unlock
  spinlock_unlock(&item_lock);
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

  spinlock_create(&item_lock);
  spinlock_create(&producer_wait_count_lock);
  spinlock_create(&consumer_wait_count_lock);

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

  printf("Producer Wait Time: %d\t\tConsumer Wait Time: %d\n",
         producer_wait_count, consumer_wait_count);

  printf("Histogram: [ ");
  int sum = 0;
  for (int i = 0; i < MAX_ITEMS + 1; i++) {
    sum += histogram[i];
    printf("%d ", histogram[i]);
  }
  printf("]\tSum: %d\n", sum);
}
