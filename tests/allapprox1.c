#include <stdio.h>

typedef struct {
  int x;
  int y;
} Point;

Point pts[10];
int ints[10];

/* All Adds could be approximated */
int main() {
  Point p;
  int arg1;
  arg1 = p.x + p.y;
  p.x = arg1 + 3;
  p.y = p.x + 5 * p.y;
  ints[0] = arg1;
  pts[0] = p;
  return 0;
}
