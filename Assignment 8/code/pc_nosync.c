#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

const int MAX_ITEMS = 10;
int       items     = 0;

const int NUM_ITERATIONS = 200;
const int NUM_CONSUMERS  = 2;
const int NUM_PRODUCERS  = 2;

void produce() {
  // TODO ensure proper synchronization
  items++;
  assert (items <= MAX_ITEMS);
}

void consume() {
  // TODO ensure proper synchronization
  items--;
  assert (items > 0);
}

void producer() {
  for (int i=0; i < NUM_ITERATIONS; i++)
    produce();
}

void consumer() {
  for (int i=0; i< NUM_ITERATIONS; i++)
    consume();
}

int main (int argc, char** argv) {
  // TODO create threads to run the producers and consumers
}