int x[8] = {1, 2, 3, 0xFFFFFFFF, 0xFFFFFFFE, 0, 184, 340057058};
int y[8];

int f(int n) {
  int count = 0;
  while (n != 0) {
    if ((n & 0x80000000) != 0) {
      count++;
    }
    n <<= 1;
  }
  return count;
}

int main(void) {
  int i = 8;
  while (i != 0) {
    i--;
    y[i] = f(x[i]);
  }
  return y[0];    // value in r0 corresponds to f(x[0]), which is stored in y[0]
}
