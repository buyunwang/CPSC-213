int param1;
int param2;
int param3;
int val;

int foo(int a, int b, int c) {
  int tmp;
  switch(a) {
    case 10:
      tmp = b + c;
      break;
    case 12:
      tmp = b - c;
      break;
    case 14:
      tmp = (b > c) ? 1 : 0;
      break;
    case 16:
      tmp = (b < c) ? 1 : 0;
      break;
    case 18:
      tmp = (b == c) ? 1 : 0;
      break;
    default:
      tmp = 0;
      break;
  }
  return tmp;
}

int main(void) {
  val = foo(param1, param2, param3);
}
