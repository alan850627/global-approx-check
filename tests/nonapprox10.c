#include <stdio.h>

typedef struct {
  int x;
  int y;
} Point;

Point pts[10];
int ints[10];

/* none of the adds could be approximated */
int main() {
  int arg1; int* address;
  address[1] = arg1 + 123;
  ints[address[1]] = 5;
  return 0;
}
