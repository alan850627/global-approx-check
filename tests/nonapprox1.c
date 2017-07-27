#include <stdio.h>

typedef struct {
  int x;
  int y;
} Point;

Point pts[10];
int ints[10];

/* none of the adds could be approximated */
int main() {
  int arg1; Point p;
  arg1 += 1;
  p.x = arg1 + p.y;
  p.y = arg1 + p.x;
  ints[arg1] = 10;
  pts[p.x].x = p.x;
  pts[p.y].y = p.y;
  return 0;
}
