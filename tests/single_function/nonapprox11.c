#include <stdio.h>

typedef struct {
  int x;
  int y;
} Point;

Point pts[10];
int ints[10];

/* none of the adds could be approximated */
int a = 0;
int main() {
  a = a + 10;
  ints[a] = 123;
  return 0;
}
