
int foo (int x) {
  return x + 1;
}

int main(void) {
  int x = 0;
  return foo (x + 1);
}

/*
#Function: main
 *Parents:
 *Childs:
   -foo
 *Critical Addresses:
 *Global Variables:
 *Arguments:
 *Instructions:
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = add nsw i32 %3, 1
  %5 = call i32 @foo(i32 %4)
  ret i32 %5

#Function: foo
 *Parents:
   -main
 *Childs:
 *Critical Addresses:
 *Global Variables:
 *Arguments:
  %2 = alloca i32, align 4
 *Instructions:
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = add nsw i32 %3, 1
  ret i32 %4
  */
