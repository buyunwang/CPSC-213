public class A {
  public static void c() {}
  public static void b (int a0, int a1) {
    int l0 = a0;
    int l1 = a1;
    A.c();
  }
}

public class Foo {
  static void foo () {
    A.b (0, 1);
  }
}
