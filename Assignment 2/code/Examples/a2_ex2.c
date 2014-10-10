int a;
int b[4];
int c;
int d[8];

c = b[0] + ((d[a & 7] + b[c & 3])&42) + 1;
