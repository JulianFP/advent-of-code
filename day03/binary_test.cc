#include <iostream>

int main() {
  char a = ' ';
  char b;
  std::cin >> b;
  int i;
  for (i = 1; i < 8; i++) {
      printf("%d", !!((a << i) & 0x80));
  }
  printf("\n");
  for (i = 1; i < 8; i++) {
      printf("%d", !!((b << i) & 0x80));
  }
  printf("\n");

  return 0;
}

