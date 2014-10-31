import static java.lang.System.out;

public class SA_dynamic_call {

  public class A {
    void ping () { out.printf ("A.ping\n"); }
    void pong () { out.printf ("A.pong\n"); }
  }
  
  public class B extends A {
    void ping () { out.printf ("B.ping\n"); }
    void wiff () { out.printf ("B.wiff\n"); }
  }
  
  public class C extends B {
    int id;
    C (int id) {
      this.id = id;
    }
    void ping () { out.printf ("C.ping(%d) calls ",id); super.ping(); }
  }
  
  static void foo (A a) {
    a.ping ();
    a.pong ();
  }

  void bar () {
    foo (new A());
    foo (new B());
    foo (new C(100));
  }

  public static void main (String args []) {
   (new SA_dynamic_call ()).bar ();
  }
}