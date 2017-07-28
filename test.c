
int foo (int x) {
  return foo(x+1) + 1;
}

int main(void) {
  int x = foo(-1000);
  return x;
}
