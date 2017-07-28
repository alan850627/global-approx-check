#include <stdio.h>

typedef struct {
  int x;
  int y;
} Point;

Point pts[10];
int ints[10];

int main() {
  int arg1;
  Point p;
  arg1 = p.x + p.y;
  p.y = p.x + 5 * p.y;
  ints[p.x] = arg1;
  pts[p.x].y = p.y;
  return 0;
}
