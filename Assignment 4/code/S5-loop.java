public class Foo {
  static int s = 0;
  static int i;
  static int a[] = new int[10];
  
  static void foo () {
    for (i=0; i<10; i++) 
      s += a[i];
  }
}