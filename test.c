
// Global Variables
int x[] = {0,1,2,3,4,5};
int y = 5;







int main(void) {
  int a = 10;
  int b = 20;
  int c = 30;
  int d[10];
  a = b + c;
  b = b + c;
  c = a + b;
  d[b] = a;
  a = b + a;
  x[a] = d[2];
  return c;
}

