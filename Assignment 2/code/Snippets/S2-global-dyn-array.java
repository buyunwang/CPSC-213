public class Foo {
  static int a;
  static int b[] = new int[10];
  
  void foo () {
    a=0;
    b[a]=a;
  }
}