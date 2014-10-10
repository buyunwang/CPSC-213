#include <stdio.h>

int main (int argc, char* argv[]) {
  short num = 0x0001;
  char* i = (char*) &num;

  printf(i[0] ? "Little Endian\n" : "Big Endian\n");
}
