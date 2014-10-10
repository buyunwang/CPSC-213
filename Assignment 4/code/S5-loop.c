#include <stdlib.h>

int  s=0;
int  i;
int a[] = {2,4,6,8,10,12,14,16,18,20};

void foo () {
  for (i=0; i<10; i++) 
    s += a[i];
}
