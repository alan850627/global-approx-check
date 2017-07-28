int foo5 (int x) {
  return x + 1;
}
int foo4 (int x) {
  return foo5(x);
}
int foo3 (int x) {
  return x + 1;
}
int foo2 (int x) {
  return foo3(x);
}
int foo1 (int x) {
  return foo4(x);
}
int foo (int x) {
  return foo1(x);
}

int main(void) {
  int x[10];
  int y = foo(0);
  int z = foo2(y);
  return x[z];
}

/*
#Function: main
 *Parents:
 *Childs:
   -foo
   -foo2
 *Critical Addresses:
  %4 = alloca i32, align 4 //NotApproxable:0
  %3 = alloca i32, align 4 //NotApproxable:0
 *Global Variables:
 *Arguments:
 *Instructions:
  %1 = alloca i32, align 4
  %2 = alloca [10 x i32], align 16
  %3 = alloca i32, align 4 //NotApproxable:0
  %4 = alloca i32, align 4 //NotApproxable:0
  store i32 0, i32* %1, align 4
  %5 = call i32 @foo(i32 0) //NotApproxable:0
  store i32 %5, i32* %3, align 4 //NotApproxable:1
  %6 = load i32, i32* %3, align 4 //NotApproxable:1
  %7 = call i32 @foo2(i32 %6) //NotApproxable:0
  store i32 %7, i32* %4, align 4 //NotApproxable:1
  %8 = load i32, i32* %4, align 4 //NotApproxable:1
  %9 = sext i32 %8 to i64 //NotApproxable:1
  %10 = getelementptr inbounds [10 x i32], [10 x i32]* %2, i64 0, i64 %9 //NotApproxable:1
  %11 = load i32, i32* %10, align 4
  ret i32 %11

#Function: foo
 *Parents:
   -main
 *Childs:
   -foo1
 *Critical Addresses:
 *Global Variables:
 *Arguments:
  %2 = alloca i32, align 4
 *Instructions:
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = call i32 @foo1(i32 %3) //NotApproxable:0
  ret i32 %4 //NotApproxable:1

#Function: foo1
 *Parents:
   -foo
 *Childs:
   -foo4
 *Critical Addresses:
 *Global Variables:
 *Arguments:
  %2 = alloca i32, align 4
 *Instructions:
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = call i32 @foo4(i32 %3) //NotApproxable:0
  ret i32 %4 //NotApproxable:1

#Function: foo4
 *Parents:
   -foo1
 *Childs:
   -foo5
 *Critical Addresses:
  %2 = alloca i32, align 4 //NotApproxable:0
 *Global Variables:
 *Arguments:
  %2 = alloca i32, align 4 //NotApproxable:0
 *Instructions:
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4 //NotApproxable:1
  %4 = call i32 @foo5(i32 %3) //NotApproxable:0
  ret i32 %4 //NotApproxable:1

#Function: foo5
 *Parents:
   -foo4
 *Childs:
 *Critical Addresses:
  %2 = alloca i32, align 4 //NotApproxable:1
 *Global Variables:
 *Arguments:
  %2 = alloca i32, align 4 //NotApproxable:1
 *Instructions:
  store i32 %0, i32* %2, align 4 //NotApproxable:1
  %3 = load i32, i32* %2, align 4 //NotApproxable:1
  %4 = add nsw i32 %3, 1 //NotApproxable:1
  ret i32 %4 //NotApproxable:1

#Function: foo2
 *Parents:
   -main
 *Childs:
   -foo3
 *Critical Addresses:
  %2 = alloca i32, align 4 //NotApproxable:1
 *Global Variables:
 *Arguments:
  %2 = alloca i32, align 4 //NotApproxable:1
 *Instructions:
  store i32 %0, i32* %2, align 4 //NotApproxable:1
  %3 = load i32, i32* %2, align 4 //NotApproxable:1
  %4 = call i32 @foo3(i32 %3) //NotApproxable:0
  ret i32 %4 //NotApproxable:1

#Function: foo3
 *Parents:
   -foo2
 *Childs:
 *Critical Addresses:
  %2 = alloca i32, align 4 //NotApproxable:1
 *Global Variables:
 *Arguments:
  %2 = alloca i32, align 4 //NotApproxable:1
 *Instructions:
  store i32 %0, i32* %2, align 4 //NotApproxable:1
  %3 = load i32, i32* %2, align 4 //NotApproxable:1
  %4 = add nsw i32 %3, 1 //NotApproxable:1
  ret i32 %4 //NotApproxable:1

  */
