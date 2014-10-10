publc class Foo {
  static int a=1;
  static int b=2;
  static int max;

  void foo () {
    if (a>b)
      max = a;
    else
      max = b;
  }
}
