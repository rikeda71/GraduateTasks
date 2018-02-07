class Parent {
  void intro() {
    System.out.println("親です");
  }
}

class Child extends Parent {
  @Override
  void intro() {
    System.out.println("子供です");
  }
}

public class Dynamic_Binding{
  public static void main(String args[]) {
    Parent ins1 = new Parent();
    Child ins2 = new Child();
    ins1.intro();
    ins2.intro();
  }
}
