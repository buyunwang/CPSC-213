int arr[10];
int *ptr = arr;

void add_at(int val, int ind) {
  ptr[ind] += val;
}

int main(void) {
  int a = 1;
  int b = 2;
  add_at(3, 4);
  add_at(a, b);
  return 12;      // value in r0 corresponds to size of add_at's frame (12)
}
