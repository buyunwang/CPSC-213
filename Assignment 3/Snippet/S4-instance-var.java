public class D {
  int e;
  int f;
  public void foo () {
    e = f;
  }
}

public class Foo {
  static D d = new D ();
  public void foo () {
    d.e = d.f;
  }
}
