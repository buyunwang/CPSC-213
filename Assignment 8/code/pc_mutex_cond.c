#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define MAX_ITEMS 10
#define NUM_ITERATIONS 250000
#define NUM_CONSUMERS 2
#define NUM_PRODUCERS 2

static int items = 0;
static int producer_wait_count = 0;
static int consumer_wait_count = 0;
static int histogram [MAX_ITEMS + 1];

static uthread_mutex_t mx;
static uthread_cond_t not_empty;
static uthread_cond_t not_full;

void produce() {
  uthread_mutex_lock(mx);
  while (items >= MAX_ITEMS) {
    producer_wait_count++;
    uthread_cond_wait(not_full);
  }
  items++;
  histogram[items]++;
  assert(items <= MAX_ITEMS);
  uthread_cond_signal(not_empty);
  uthread_mutex_unlock(mx);
}

void consume() {
  uthread_mutex_lock(mx);
  while (items <= 0) {
    consumer_wait_count++;
    uthread_cond_wait(not_empty);
  }
  items--;
  histogram[items]++;
  assert (items >= 0);
  uthread_cond_signal(not_full);
  uthread_mutex_unlock(mx);
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

  mx = uthread_mutex_create();
  not_empty = uthread_cond_create(mx);
  not_full = uthread_cond_create(mx);

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

  uthread_cond_destroy(not_full);
  uthread_cond_destroy(not_empty);
  uthread_mutex_destroy(mx);

  printf("Producer Wait Time: %d\tConsumer Wait Time: %d\n",
         producer_wait_count, consumer_wait_count);

  printf("Histogram: [ ");
  int sum = 0;
  for (int i = 0; i < MAX_ITEMS + 1; i++) {
    sum += histogram[i];
    printf("%d ", histogram[i]);
  }
  printf("]\tSum: %d\n", sum);
}
