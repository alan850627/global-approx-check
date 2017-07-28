#include <stdio.h>

typedef struct {
  int x;
  int y;
} Point;

Point pts[10];
int ints[10];

/* none of the adds could be approximated */
int main(){
  int arg1;
  Point p;
  if(arg1 > 0) {
    p.y = arg1 + p.x;
    pts[arg1].y = p.y;
  } else {
    p.x = arg1 + p.y;
    pts[arg1].x = p.x;
  }
  return 0;
}
