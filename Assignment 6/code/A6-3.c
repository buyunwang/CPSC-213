#include <stdio.h>

int add (a,b) {
  return a+b;
}

int sub (a,b) {
  return a-b;
}

int inc (a,b) {
  return a+1;
}

int dec (a,b) {
  return a-1;
}

struct Cmd {
  int (*op) (int,int);
  int a;
  int b;
};

struct Cmd cmds[4] = { {add, 5,2}, {sub, 5,2}, {inc, 5, 0}, {dec, 5,0} };
int soln[4];

void compute (struct Cmd* c, int s[], int n) {
  for (int i=0; i<n; i++)
    s [i] = c[i].op (c[i].a, c[i].b);
}

// not in .s file (create your own main to initialize stack cand call compute
int main (int argc, char** argv) {
  compute (cmds, soln, sizeof(cmds)/sizeof(cmds[0]));
  for (int i=0; i<sizeof(soln)/sizeof(soln[0]); i++)
    printf ("%d\n", soln[i]);
}