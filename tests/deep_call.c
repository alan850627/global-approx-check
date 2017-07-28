
int foo4 (int x) {
  return x + 1;
}
int foo3 (int x) {
  return foo4(x);
}
int foo2 (int x) {
  return foo3(x);
}
int foo1 (int x) {
  return foo2(x);
}
int foo (int x) {
  return foo1(x);
}

int main(void) {
  int x[10];
  return x[foo(0)];
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
  %2 = alloca [10 x i32], align 16
  store i32 0, i32* %1, align 4
  %3 = call i32 @foo(i32 0) //NotApproxable:0
  %4 = sext i32 %3 to i64 //NotApproxable:1
  %5 = getelementptr inbounds [10 x i32], [10 x i32]* %2, i64 0, i64 %4 //NotApproxable:1
  %6 = load i32, i32* %5, align 4
  ret i32 %6

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
   -foo2
 *Critical Addresses:
  %2 = alloca i32, align 4 //NotApproxable:0
 *Global Variables:
 *Arguments:
  %2 = alloca i32, align 4 //NotApproxable:0
 *Instructions:
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4 //NotApproxable:1
  %4 = call i32 @foo2(i32 %3) //NotApproxable:0
  ret i32 %4 //NotApproxable:1

#Function: foo2
 *Parents:
   -foo1
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
   -foo4
 *Critical Addresses:
  %2 = alloca i32, align 4 //NotApproxable:1
 *Global Variables:
 *Arguments:
  %2 = alloca i32, align 4 //NotApproxable:1
 *Instructions:
  store i32 %0, i32* %2, align 4 //NotApproxable:1
  %3 = load i32, i32* %2, align 4 //NotApproxable:1
  %4 = call i32 @foo4(i32 %3) //NotApproxable:0
  ret i32 %4 //NotApproxable:1

#Function: foo4
 *Parents:
   -foo3
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
