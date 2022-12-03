#include <iostream>

int main() {
  char a;
  std::cin >> a;
  int i;
  for (i = 0; i < 8; i++) {
      printf("%d", !!((a << i) & 0x80));
  }
  printf("\n");

  return 0;
}

