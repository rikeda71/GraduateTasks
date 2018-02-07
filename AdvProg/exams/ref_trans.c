#include <stdio.h>

void show1() {
  int c;
  c = getchar();
  putchar(c);
}

void show2(int a) {
  // 引数はgetchar()の返り値を受け取った引数
  putchar(a);
}

int main() {
  int c;
  // 参照透明性を満たしていない例
  // show1();
  // 参照透明性を満たしている例
  c = getchar();
  show2(c);

  return 0;
}
