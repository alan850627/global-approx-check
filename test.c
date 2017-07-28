
// Global Variables
int x[] = {0,1,2,3,4,5};
int y = 5;

void foo (int* a) {
  int c = 1;
  c = c + 10;
  *a = c;
}

int main(void) {
  int a = 10;
  foo(&a);
  x[a] = 2;
  return 0;
}
