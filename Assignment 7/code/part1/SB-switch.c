#include <stdio.h>

int i, j;

int foo () {
  switch (i) {
    case 20:  j=10; break;
    case 21:  j=11; break;
    case 23:  j=13; break;
    default:  j=14; break;
  }
  return j;
}

int foo_using_if () {
  if (i==20)
    j=10;
  else if (i==21) 
    j = 11;
  else if (i==22) 
    j = 14;
  else if (i==23)
    j = 13;
  else
    j = 14;
  
  return j;
}


int foo_using_jumptable () {
  static const void* jumpTable[4] = { &&L20, &&L21, &&DEFAULT, &&L23 };
  
  if (i < 20 || i > 23) goto DEFAULT;
  goto *jumpTable [i-20];
L20:
  j = 10;
  goto CONT;
L21:
  j = 11;
  goto CONT;
L23:
  j = 13;
  goto CONT;
DEFAULT:
  j = 14;
  goto CONT;
CONT:
  return j;
}

int main (int argc, char** argv) {
  for (i=19; i<=24; i++)
    printf ("%d: %d %d %d\n", i, foo(), foo_using_if(), foo_using_jumptable());
}