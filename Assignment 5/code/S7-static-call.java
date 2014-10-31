public class A {
  static void ping () {}
}

public class Foo {
  static void foo () {
    A.ping ();
  }
}