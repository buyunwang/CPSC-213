int a;
int b;
int c[8];
int *d = &c;


c[b] = ((c[a] + 1) >> 1) - 1;
b = b + 4;
d[b] = d[a] - 4;
