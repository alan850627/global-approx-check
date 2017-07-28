
// Global Variables
int x[] = {0,1,2,3,4,5};
int y = 5;

void foo (int* a) {
  int c = 1;
  c = c + 10;
  *a = c;
  x[*a] = 1;
}

int main(void) {
  int a = 10;
  foo(&a);
  x[a] = 2;
  return 0;
}

/*
#Function: main
 *Parents:
 *Childs:
   -foo
 *Critical Addresses:
  %2 = alloca i32, align 4 //NotApproxable:0
 *Global Variables:
  @x = global [6 x i32] [i32 0, i32 1, i32 2, i32 3, i32 4, i32 5], align 16
 *Arguments:
 *Instructions:
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4 //NotApproxable:0
  store i32 0, i32* %1, align 4
  store i32 10, i32* %2, align 4 //NotApproxable:1
  call void @foo(i32* %2)
  %3 = load i32, i32* %2, align 4 //NotApproxable:1
  %4 = sext i32 %3 to i64 //NotApproxable:1
  %5 = getelementptr inbounds [6 x i32], [6 x i32]* @x, i64 0, i64 %4 //NotApproxable:1
  store i32 2, i32* %5, align 4
  ret i32 0

#Function: foo
 *Parents:
   -main
 *Childs:
 *Critical Addresses:
  %2 = alloca i32*, align 8 //NotApproxable:1
  %8 = load i32*, i32** %2, align 8 //NotApproxable:1
  %3 = alloca i32, align 4 //NotApproxable:0
 *Global Variables:
  @x = global [6 x i32] [i32 0, i32 1, i32 2, i32 3, i32 4, i32 5], align 16
 *Arguments:
  %2 = alloca i32*, align 8 //NotApproxable:1
 *Instructions:
  %3 = alloca i32, align 4 //NotApproxable:0
  store i32* %0, i32** %2, align 8 //NotApproxable:1
  store i32 1, i32* %3, align 4 //NotApproxable:1
  %4 = load i32, i32* %3, align 4 //NotApproxable:1
  %5 = add nsw i32 %4, 10 //NotApproxable:1
  store i32 %5, i32* %3, align 4 //NotApproxable:1
  %6 = load i32, i32* %3, align 4 //NotApproxable:1
  %7 = load i32*, i32** %2, align 8 //NotApproxable:1
  store i32 %6, i32* %7, align 4 //NotApproxable:1
  %8 = load i32*, i32** %2, align 8 //NotApproxable:1
  %9 = load i32, i32* %8, align 4 //NotApproxable:1
  %10 = sext i32 %9 to i64 //NotApproxable:1
  %11 = getelementptr inbounds [6 x i32], [6 x i32]* @x, i64 0, i64 %10 //NotApproxable:1
  store i32 1, i32* %11, align 4
  ret void 

  */
