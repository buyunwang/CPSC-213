#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

struct sumContext {
  int sum;
};

struct minContext {
  int min;
};

struct maxContext {
  int max;
};

struct sizeContext {
  int size;
};

struct meanContext {
  double mean;
  int n;
};

struct medianContext {
  double median;
  int* array;
  int size;
  int capacity;
};

void sum(int element, void* contextv) {
  struct sumContext* context = (struct sumContext*) contextv;
  context->sum += element;
}

void min(int element, void* contextv) {
  struct minContext* context = (struct minContext*) contextv;
  if (element < context->min) {
    context->min = element;
  }
}

void max(int element, void* contextv) {
  struct maxContext* context = (struct maxContext*) contextv;
  if (element > context->max) {
    context->max = element;
  }
}

void size(int element, void* contextv) {
  struct sizeContext* context = (struct sizeContext*) contextv;
  context->size++;
}

void mean(int element, void* contextv) {
  struct meanContext* context = (struct meanContext*) contextv;
  context->mean += (element - context->mean) / ++context->n;
}

// insert element into sorted array
void insert(int val, int* array, int size) {
  if (size == 1) {
    array[0] = val;
  } else {
    if (val < array[0]) {     //if val < head of array, swap val and array[0]
      array[0] += val;
      val = array[0] - val;
      array[0] -= val;
    }
    insert(val, array + 1, size - 1);   // insert val into rest of array
  }
}

// calculate average of two numbers
double avg(int a, int b) {
  return a + (b - a)/2.0;
}

void median(int element, void* contextv) {
  struct medianContext* context = (struct medianContext*) contextv;

  // resize array if capacity reached
  if (++context->size >= context->capacity) {
    context->capacity *= 2;
    context->array = realloc(context->array, context->capacity * sizeof(int));
  }
  // insert element into correct position in sorted array
  insert(element, context->array, context->size);

  // median is middle element of array (or average of middle elements)
  if (context->size % 2 == 0) {
    context->median = avg(context->array[context->size/2],
                          context->array[context->size/2 - 1]);
  } else {
    context->median = context->array[context->size/2];
  }
}

void iterate(int* array, int n, void* context, void (*op)(int, void*)) {
  int i;
  for (i = 0; i < n; i++) {
    op(array[i], context);
  }
}
int main(void) {
  int array[] = {1,2,3,4,5,10};

  struct sumContext context_sum = { 0 };
  struct minContext context_min = { INT_MAX };
  struct maxContext context_max = { INT_MIN };
  struct sizeContext context_size = { 0 };
  struct meanContext context_mean = { 0, 0 };
  struct medianContext context_median = {
      0, malloc(sizeof array), 0, sizeof(array)/sizeof(array[0]) };

  iterate(array, sizeof(array) / sizeof(array[0]), &context_sum, sum);
  printf("Sum: %d\n", context_sum.sum);

  iterate(array, sizeof(array) / sizeof(array[0]), &context_min, min);
  printf("Min: %d\n", context_min.min);

  iterate(array, sizeof(array) / sizeof(array[0]), &context_max, max);
  printf("Max: %d\n", context_max.max);

  iterate(array, sizeof(array) / sizeof(array[0]), &context_size, size);
  printf("Size: %d\n", context_size.size);

  iterate(array, sizeof(array) / sizeof(array[0]), &context_mean, mean);
  printf("Mean: %.2f\n", context_mean.mean);

  iterate(array, sizeof(array) / sizeof(array[0]), &context_median, median);
  printf("Median: %.2f\n", context_median.median);
  free(context_median.array);
}
