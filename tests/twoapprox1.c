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
  int i = 0;
  for (int i = 0; i < 1000; i++) {
    ints[i] = p.x + p.y + i;
  }
  return 0;
}
