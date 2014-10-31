import static java.lang.System.out;

public class A6_1 {

  class Person {
    String name;
    Person (String name) {
      this.name = name;
    }
    public String toString() {
      return "Name: " .concat (name);
    }
  }
  
  class Student extends Person {
    Integer sid;
    Student (String name, Integer sid) {
      super (name);
      this.sid = sid;
    }
    public String toString() {
      return super.toString() .concat (", SID: ") .concat (sid.toString());
    }
  }
  
  void run() {
    Person[] people = {new Person ("Alex"), new Student ("Alice", 300)};
    for (Person person : people)
      out.printf ("%s\n", person.toString());
  }
  
  public static void main (String[] args) {
    (new A6_1()) .run();
  }
}