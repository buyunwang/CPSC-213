#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Class Person
 */

struct Person_class {
  void* super;
  void (* toString) (void*, char*, int);
};

struct Person {
  struct Person_class* class;
  char* name;
};

void Person_toString (void* thisVP, char* buffer, int buffer_size) {
  struct Person* this = thisVP;
  snprintf(buffer, buffer_size, "Name: %s", this->name);
}

struct Person_class Person_class_obj = {NULL, Person_toString};

struct Person* new_Person(char* name) {
  struct Person* obj = malloc (sizeof (struct Person));
  obj->class = &Person_class_obj;
  obj->name = strdup(name);
  return obj;
}

void delete_Person(void* thisVP) {
  struct Person* this = thisVP;
  free(this->name);
  free(this);
}


/*
 * Class Student extends Person
 */

struct Student_class {
  struct Person_class* super;
  void (* toString) (void*, char*, int);
};

struct Student {
  struct Student_class* class;
  char* name;
  int sid;
};

void Student_toString(void* thisVP, char* buffer, int buffer_size) {
  struct Student* this = thisVP;

  char super_string[buffer_size];
  this->class->super->toString(this, super_string, buffer_size);

  snprintf(buffer, buffer_size, "%s, SID: %i", super_string, this->sid);
}

struct Student_class Student_class_obj = {&Person_class_obj, Student_toString};


struct Student* new_Student(char* name, int sid) {
  struct Student* obj = malloc (sizeof (struct Student));
  obj->class = &Student_class_obj;
  obj->name = strdup(name);
  obj->sid = sid;
  return obj;
}

void delete_Student(void* thisVP) {
  struct Student* this = thisVP;
  free(this->name);
  free(this);
}

/*
 * Main
 */

void run() {
  struct Person *people[2] = {new_Person("Alex"),
                              (struct Person *) new_Student("Alice", 300)};
  int i;
  for (i = 0; i < (sizeof people)/(sizeof people[0]); i++) {
    char buf[128];
    people[i]->class->toString(people[i], buf, sizeof buf);
    printf("%s\n", buf);
  }

  delete_Person(people[0]);
  delete_Student(people[1]);
}

int main (int argc, char** argv) {
  run();
}
