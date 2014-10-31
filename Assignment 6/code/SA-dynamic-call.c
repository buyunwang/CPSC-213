#include <stdlib.h>
#include <stdio.h>

/*
 * Class A
 */

struct A_class {
  void*   super;
  void (* ping ) (void*);
  void (* pong ) (void*);
};

struct A {
  struct A_class* class;
};

void A_ping (void* this) { printf ("A_ping\n"); }
void A_pong (void* this) { printf ("A_pong\n"); }

struct A_class A_class_obj = {NULL, A_ping, A_pong};

struct A* new_Person() {
  struct A* obj = malloc (sizeof (struct A));
  obj->class = &A_class_obj;
  return obj;
}


/*
 * Class B extends A
 */

struct B_class {
  struct A_class* super;
  void         (* ping ) (void*);
  void         (* pong ) (void*);
  void         (* wiff ) (void*);
};

struct B {
  struct B_class* class;
};

void B_ping (void* this) { printf ("B_ping\n"); }
void B_wiff (void* this) { printf ("B_wiff\n"); }

struct B_class B_class_obj = {&A_class_obj, B_ping, A_pong, B_wiff};

struct B* new_B() {
  struct B* obj = malloc (sizeof (struct B));
  obj->class = &B_class_obj;
  return obj;
}


/*
 * Class C extends B
 */

struct C_class {
  struct B_class* super;
  void         (* ping ) (void*);
  void         (* pong ) (void*);
  void         (* wiff ) (void*);
};

struct C {
  struct C_class* class;
  int    id;
};

void C_ping (void* thisVP) {
  struct C* this = thisVP;
  printf ("C_ping (%d) calls ", this->id);
  this->class->super->ping (this);
}

struct C_class C_class_obj = {&B_class_obj, C_ping, A_pong, B_wiff};

struct C* new_C (int id) {
  struct C* obj = malloc (sizeof (struct C));
  obj->class = &C_class_obj;
  obj->id    = id;
  return obj;
}


/*
 * Main
 */

void foo (void* aVP) {
  struct A* a = aVP;
  a->class->ping (a);
  a->class->pong (a);
}

void bar() {
  foo (new_Person());
  foo (new_B());
  foo (new_C(100));
}

int main (int argc, char** argv) {
  bar();
}
