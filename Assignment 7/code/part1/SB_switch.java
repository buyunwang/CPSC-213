import static java.lang.System.out;

public class SB_switch {
  static int i,j;
  
  static int foo () {
    switch (i) {
      case 20: j=10; break;
      case 21: j=11; break;
      case 23: j=13; break;
      default: j=14; break;
    }
    return j;
  }
  
  public static void main (String[] args) {
    for (i=19; i<=24; i++)
      out.printf ("%d: %d\n", i, foo());
  }
}
