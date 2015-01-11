#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include "uthread.h"
#include "uthread_sem.h"

#define N 20
#define ITERATIONS 100
#define CAPACITY 3
#define MALE 1
#define FEMALE 0
#define SWITCHING 1
#define WAITING_HISTOGRAM_SIZE 20

static uthread_t threads[N];
static uthread_sem_t turnstile;
static uthread_sem_t is_empty;
static int gender_counter[2];
static uthread_sem_t gender_counter_lock[2];
static uthread_sem_t count_from_full[2];
static uthread_sem_t stat_lock;

static int num_of_gender[2];
static int using[2] = {0, 0};
static int occupancy[2][3] = {{0, 0, 0}, {0, 0, 0}};
static int counter = 0;
static int waiting_histogram[WAITING_HISTOGRAM_SIZE];
static int waiting_histogram_overflow = 0;

void washroom_enter(const int gender) {
  uthread_sem_wait(stat_lock);
  counter++;
  int start = counter;
  uthread_sem_signal(stat_lock);

  uthread_sem_wait(turnstile);
  uthread_sem_wait(gender_counter_lock[gender]);
  if (++gender_counter[gender] == 1) {
    uthread_sem_wait(is_empty);
  }

  uthread_sem_signal(gender_counter_lock[gender]);
  uthread_sem_signal(turnstile);
  uthread_sem_wait(count_from_full[gender]);

  uthread_sem_wait(stat_lock);
  using[gender]++;
  occupancy[gender][using[gender]-1]++;
  if (counter - start < WAITING_HISTOGRAM_SIZE) {
    waiting_histogram[counter-start]++;
  }
  else {
    waiting_histogram_overflow++;
  }
  uthread_sem_signal(stat_lock);
}

void washroom_exit(const int gender) {
  uthread_sem_signal(count_from_full[gender]);
  uthread_sem_wait(gender_counter_lock[gender]);
  using[gender]--;
  if (--gender_counter[gender] == 0) {
    uthread_sem_signal(is_empty);
  }
  uthread_sem_signal(gender_counter_lock[gender]);
}

void* person(void* gender_vp) {
  const int gender = (intptr_t) gender_vp;
  for (int iter = 0; iter < ITERATIONS; iter++) {
    washroom_enter(gender);
    for (int i = 0; i < N; i++) {
      uthread_yield();
    }
    washroom_exit(gender);
    for (int i = 0; i < N; i++) {
      uthread_yield();
    }
  }
  return NULL;
}

void print_occupancy() {
  int sum;
  printf("\t1 \t2 \t3 \tT\n");
  printf("Male:");
  for (int i = 0; i < 3; i++) {
    printf("\t%d", occupancy[MALE][i]);
  }
  sum = occupancy[MALE][0] + occupancy[MALE][1] + occupancy[MALE][2];
  printf("\t%d\nFemale:", sum);
  for (int i = 0; i < 3; i++) {
    printf("\t%d", occupancy[FEMALE][i]);
  }
  sum = occupancy[FEMALE][0] + occupancy[FEMALE][1] + occupancy[FEMALE][2];
  printf("\t%d\nTotal:", sum);
  sum = 0;
  for (int i = 0; i < 3; i++) {
    printf("\t%d", occupancy[MALE][i] + occupancy[FEMALE][i]);
    sum += occupancy[MALE][i] + occupancy[FEMALE][i];
  }
  printf("\t%d\n", sum);
}

void print_histogram() {
  printf("Histogram: [ ");
  for (int i = 0; i < WAITING_HISTOGRAM_SIZE; i++) {
    printf("%d ", waiting_histogram[i]);
  }
  printf("]\nOverflow: %d\n", waiting_histogram_overflow);
}

int main (int argc, char** argv) {
  srand(time(NULL));
  // setup semaphores
  turnstile = uthread_sem_create(1);
  is_empty = uthread_sem_create(1);
  stat_lock = uthread_sem_create(1);

  gender_counter[MALE] = 0;
  gender_counter_lock[MALE] = uthread_sem_create(1);
  count_from_full[MALE] = uthread_sem_create(CAPACITY);

  gender_counter[FEMALE] = 0;
  gender_counter_lock[FEMALE] = uthread_sem_create(1);
  count_from_full[FEMALE] = uthread_sem_create(CAPACITY);

  // setup threads
  uthread_init(8);
  num_of_gender[MALE] = rand() % N;
  num_of_gender[FEMALE] = N - num_of_gender[MALE];
  printf("Created %d male thread(s) and %d female thread(s), "
      "running %d iterations each.\n",
      num_of_gender[MALE], num_of_gender[FEMALE], ITERATIONS);
  for (int i = 0; i < num_of_gender[MALE]; i++) {
    threads[i] = uthread_create(person, (void*) MALE);
  }
  for (int i = num_of_gender[MALE]; i < N; i++) {
    threads[i] = uthread_create(person, (void*) FEMALE);
  }
  // join all threads before completion
  for (int i = 0; i < N; i++) {
    uthread_join(threads[i], NULL);
  }
  // print out summary stats
  print_occupancy();
  print_histogram();
  return 0;
}
