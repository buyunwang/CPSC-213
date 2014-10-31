#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct concatContext {
  char* buf;
  int size;
};

void concat(void* elementv, void* contextv) {
  char* string = elementv;
  struct concatContext* context = contextv;
  strcat(context->buf, string);
}


void iterate(void* array[], int n, void* context, void (*op)(void*, void*)) {
  int i;
  for (i = 0; i < n; i++) {
    op(array[i], context);
  }
}

int string_array_size(char* array[], int n) {
  size_t size = 0;
  int i;
  for (i = 0; i < n; i++) {
    size += strlen(array[i]) + 1;
  }
  return size;
}

int main(void) {
  const int N = 6;
  char* array[] = {"foo", "bar", "baz", "abcdef", "ghijklmnop", "123"};
  struct concatContext context = { calloc(string_array_size(array, N), 1), 0 };
  iterate((void**) array, N, &context, concat);
  printf("Concatenated string: %s\n", context.buf);
  free(context.buf);
}
