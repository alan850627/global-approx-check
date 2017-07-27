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
  while (arg1 > 0) {
    arg1 += 1;
    pts[arg1].y = p.y;
  }
  return 0;
}
