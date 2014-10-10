#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//////////////////////
//
// STACK

#define STACK_NAME_SIZE 100

struct StackElement {
  char   fill[8];
  char   name[STACK_NAME_SIZE];
  struct StackElement *next;
};

struct StackElement *stackTop = 0;

void push (char* aName) {
  struct StackElement* e = (struct StackElement*) malloc (sizeof (struct StackElement));
  strncpy (e->name, aName, STACK_NAME_SIZE);
  e->next  = stackTop;
  stackTop = e;
}

char* pop () {
  struct StackElement* e = stackTop;
  stackTop = e->next;
  free (e);
  return e->name;
}

///
//////////////////////


//////////////////////
//
// TEST CASES

#define INT_ARRAY_SIZE (sizeof(struct StackElement)/sizeof(int))

//
// Correctly prints "Zero"
//
void test1 () {
  printf ("test1:\n");
  int* ip = (int *) malloc (INT_ARRAY_SIZE*sizeof(int));
  int* ipc = ip;
  free (ip);
  push ("Zero");
  printf ("%s\n", pop ());
}

//
// Should produce the same output as test1, but it fails
//
void test2 () {
  printf ("test2:\n");
  int* ip = (int *) malloc (INT_ARRAY_SIZE*sizeof(int));
  int* ipc = ip;
  free (ip);
  push ("Zero");
  while (ipc - ip < INT_ARRAY_SIZE) 
    *(ipc++) = 0;
  printf ("%s\n", pop ());
}

//
// Correctly prints "One", "Zero"
//
void test3 () {
  printf ("test3:\n");
  push ("Zero");
  int* ip = (int *) malloc (INT_ARRAY_SIZE*sizeof(int));
  int* ipc = ip;
  free (ip);
  push ("One");
  printf ("%s\n", pop ());
  printf ("%s\n", pop ());
}

//
// Should produce same output as test3, but it fails
//
void test4 () {
  printf ("test4:\n");
  push ("Zero");
  int* ip = (int *) malloc (INT_ARRAY_SIZE*sizeof(int));
  int* ipc = ip;
  free (ip);
  push ("One");
  while (ipc - ip < INT_ARRAY_SIZE) 
    *(ipc++) = 0;
  printf ("%s\n", pop ());
  printf ("%s\n", pop ());
}

//
// Correctly prints "One", "Zero", "Three", "Two"
//
void test5 () {
  printf ("test5:\n");
  push ("Zero");
  push ("One");
  printf ("%s\n", pop ()); 
  printf ("%s\n", pop ());
  push("Two");
  push("Three");
  printf ("%s\n", pop ());
  printf ("%s\n", pop ());
}

//
// Should produce the same output as test5, but if fails
//
void test6 () {
  printf ("test6:\n");
  char *x[2];
  push ("Zero");
  push ("One");
  x[0] = pop ();
  x[1] = pop ();
  push("Two");
  push("Three");
  printf ("%s\n", x[0]);
  printf ("%s\n", x[1]);
  printf ("%s\n", pop());
  printf ("%s\n", pop());
}

//
// Should print list of "string #" for every number from numPushes-1 down to 0,
// in descending order, for any value of numPushes > 0
//
void test7 (int numPushes) {
  char  s[100];
  char* t;
  int   i;
  
  for (i=0; i<numPushes; i++) {
    snprintf (s, sizeof (s), "string %d", i);
    push (s);
  }
  for (i=0; i<numPushes; i++) {
    t = pop ();
    printf ("%s\n", t);
  }
}

//
// on linux, run ulimit -v 10000 before running test
// to be sure deallocations are occuring
// that limits max vm size to 10000 kb, which is around 9-MB
// if test8 prints "okay" then stack elements are being freed
// if you instead get a "segmentation fault", they are not
//
void test8() {
  char s[100];
  int i;
  for (i=0; i<100000; i++) {
    push ("hello");
    pop  ();
  }
  printf ("okay\n");
}

//
//////////////////////


//////////////////////
//
// MAIN LINE

#define USAGE "usage: dangling-pointer test-number (where test-number is 1-8)"

int main (int argc, char** argv) {
  if (argc < 2) {
    printf ("%s\n", USAGE);
    exit (EXIT_FAILURE);
  }
  switch (strtol (argv[1],0,10)) {
    case 1:
      test1 ();
      break;
    case 2:
      test2 ();
      break;
    case 3:
      test3 ();
      break;
    case 4:
      test4 ();
      break;
    case 5:
      test5 ();
      break;
    case 6:
      test6 ();
      break;
    case 7:
      if (argc != 3) {
        printf ("%s numPushes\n", USAGE);
        exit (EXIT_FAILURE);
      }
      test7 (strtol (argv[2] ,0, 10));
      break;
    case 8:
      test8();
      break;
    default:
      printf ("%s\n", USAGE);
      exit (EXIT_FAILURE);
  }
}
