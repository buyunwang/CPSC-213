int a[8];
int b[8];
int c[8];
int i;
int *d = &c;

a[i] = a[i+1] + b[i+2];
d[i] = a[i] + b[i]
d[i] = a[b[i]] + b[a[i]];
d[b[i]] = b[a[i & 3] & 3] - a[b[i & 3] & 3] + d[i];
