void c() {}

void b (int a0, int a1) {
  int l0 = a0;
  int l1 = a1;
  c();
}

void foo () {
  b (0, 1);
}
