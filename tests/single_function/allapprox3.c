#include <stdio.h>

typedef struct {
  int x;
  int y;
} Point;

Point pts[10];
int ints[10];

int main() {
  int arg1; Point p;
  p.x = arg1 + 3;
  p.y = p.x + 5 * p.y;
  ints[arg1] = arg1;
  pts[arg1].y = p.y;
  return 0;
}
