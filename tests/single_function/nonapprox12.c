#include <stdio.h>

typedef struct {
  int x;
  int y;
} Point;

Point pts[10];
int ints[10];

/* none of the adds could be approximated */
int main() {
  int* addr; int x;
  addr[x+x] = x + 10;
  ints[addr[x+x]] = 12345;
  return 0;
}
