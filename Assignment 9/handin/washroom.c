#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define N 20
#define ITERATIONS 100
#define MALE 1
#define FEMALE 0
#define SWITCHING 1
#define WAITING_HISTOGRAM_SIZE 20

static uthread_t threads[N];
static uthread_mutex_t mx;
static uthread_cond_t try_enter[2];

static int num_of_gender[2];
static int occupancy[2][3] = {{0, 0, 0}, {0, 0, 0}};
static int counter = 0;
static int waiting_histogram[WAITING_HISTOGRAM_SIZE];
static int waiting_histogram_overflow = 0;

static struct washroom {
  int gender;
  int using[2];
  int queue[2];
  int state;
} wr = {-1, {0, 0}, {0, 0}, !SWITCHING};

void washroom_enter(const int gender) {
  uthread_mutex_lock(mx);
  counter++;
  int start = counter;
  if (wr.gender == -1) {
    // first run
    wr.gender = gender;
  }
  // block until conditions are met
  while (wr.using[gender] >= 3
      || wr.gender != gender
      || wr.state == SWITCHING) {
    wr.queue[gender]++;
    uthread_cond_wait(try_enter[gender]);
    wr.queue[gender]--;
    if (wr.using[gender] + wr.using[!gender] == 0) {
      wr.gender = gender;
    }
  }
  // successfully entered
  wr.using[gender]++;
  // test for correctness, update histograms/counters
  assert(1 <= wr.using[gender] && wr.using[gender] <= 3);
  assert(wr.using[!gender] == 0);
  occupancy[gender][wr.using[gender]-1]++;
  if (counter - start < WAITING_HISTOGRAM_SIZE) {
    waiting_histogram[counter-start]++;
  }
  else {
    waiting_histogram_overflow++;
  }
  uthread_mutex_unlock(mx);
}

void washroom_exit() {
  uthread_mutex_lock(mx);
  wr.using[wr.gender]--;
  if (wr.state == SWITCHING) {
    if (wr.using[wr.gender] == 0) {
      // cleared out gender, can switch to other
      wr.gender = !wr.gender;
      wr.state = !SWITCHING;
      uthread_cond_broadcast(try_enter[wr.gender]);
    }
  }
  else {
    if (wr.using[wr.gender] == 0) {
      // no one in washroom, let other gender try to come in
      uthread_cond_broadcast(try_enter[!wr.gender]);
    }
    else if ((wr.queue[!wr.gender] > 0)
        && rand() % (N - wr.queue[!wr.gender]) == 0) {
      // force switch policy: chance increases as queue increases
      wr.state = SWITCHING;
    }
    uthread_cond_broadcast(try_enter[wr.gender]);
  }
  uthread_mutex_unlock(mx);
}

void* person(void* gender_vp) {
  const int gender = (intptr_t) gender_vp;
  for (int iter = 0; iter < ITERATIONS; iter++) {
    washroom_enter(gender);
    for (int i = 0; i < N; i++) {
      uthread_yield();
    }
    washroom_exit();
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
  printf("\t%d\n\n", sum);
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
  // setup mutexes and condition variables
  mx = uthread_mutex_create();
  try_enter[MALE] = uthread_cond_create(mx);
  try_enter[FEMALE] = uthread_cond_create(mx);
  // setup threads
  uthread_init(8);
  num_of_gender[MALE] = rand() % N;
  num_of_gender[FEMALE] = N - num_of_gender[MALE];
  printf("Created %d male thread(s) and %d female thread(s), "
      "running %d iterations each.\n\n",
      num_of_gender[MALE], num_of_gender[FEMALE], ITERATIONS);
  for (int i = 0; i < num_of_gender[MALE]; i++) {
    threads[i] = uthread_create(person, (void*) MALE);
  }
  for (int i = num_of_gender[MALE]; i < N; i++) {
    threads[i] = uthread_create(person, (void*) FEMALE);
  }
  // join all threads before completion
  for (int i = 0; i < (sizeof threads) / (sizeof threads[0]); i++) {
    uthread_join(threads[i], NULL);
  }
  // print out summary stats
  print_occupancy();
  print_histogram();
  return 0;
}
