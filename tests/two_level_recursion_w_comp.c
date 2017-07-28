int bfoo();

int afoo(int a) {
  if (a > 0) {
    return 1;
  }
  a = bfoo(a + 1);
  return a;
}

int bfoo(int b) {
  if (b < 0) {
    return 1;
  }
  b = afoo(b + 1);
  return b;
}

int main(void) {
  int x = afoo(10);
  int y = bfoo(20);
  return x + y;
}

/*
#Function: main
 *Parents:
 *Childs:
   -afoo
   -bfoo
 *Critical Addresses:
 *Global Variables:
 *Arguments:
 *Instructions:
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  %4 = call i32 @afoo(i32 10)
  store i32 %4, i32* %2, align 4
  %5 = call i32 @bfoo(i32 20)
  store i32 %5, i32* %3, align 4
  %6 = load i32, i32* %2, align 4
  %7 = load i32, i32* %3, align 4
  %8 = add nsw i32 %6, %7
  ret i32 %8

#Function: afoo
 *Parents:
   -main
   -bfoo
 *Childs:
   -bfoo
 *Critical Addresses:
  %3 = alloca i32, align 4 //NotApproxable:0
  %2 = alloca i32, align 4 //NotApproxable:1
 *Global Variables:
 *Arguments:
  %2 = alloca i32, align 4 //NotApproxable:1
 *Instructions:
  %3 = alloca i32, align 4 //NotApproxable:0
  store i32 %0, i32* %3, align 4 //NotApproxable:1
  %4 = load i32, i32* %3, align 4 //NotApproxable:1
  %5 = icmp sgt i32 %4, 0 //NotApproxable:1
  br i1 %5, label %6, label %7
  store i32 1, i32* %2, align 4 //NotApproxable:1
  br label %12
  %8 = load i32, i32* %3, align 4 //NotApproxable:1
  %9 = add nsw i32 %8, 1 //NotApproxable:1
  %10 = call i32 @bfoo(i32 %9) //NotApproxable:0
  store i32 %10, i32* %3, align 4 //NotApproxable:1
  %11 = load i32, i32* %3, align 4 //NotApproxable:1
  store i32 %11, i32* %2, align 4 //NotApproxable:1
  br label %12
  %13 = load i32, i32* %2, align 4 //NotApproxable:1
  ret i32 %13 //NotApproxable:1

#Function: bfoo
 *Parents:
   -afoo
   -main
 *Childs:
   -afoo
 *Critical Addresses:
  %2 = alloca i32, align 4 //NotApproxable:1
  %3 = alloca i32, align 4 //NotApproxable:0
 *Global Variables:
 *Arguments:
  %2 = alloca i32, align 4 //NotApproxable:1
 *Instructions:
  %3 = alloca i32, align 4 //NotApproxable:0
  store i32 %0, i32* %3, align 4 //NotApproxable:1
  %4 = load i32, i32* %3, align 4 //NotApproxable:1
  %5 = icmp slt i32 %4, 0 //NotApproxable:1
  br i1 %5, label %6, label %7
  store i32 1, i32* %2, align 4 //NotApproxable:1
  br label %12
  %8 = load i32, i32* %3, align 4 //NotApproxable:1
  %9 = add nsw i32 %8, 1 //NotApproxable:1
  %10 = call i32 @afoo(i32 %9) //NotApproxable:0
  store i32 %10, i32* %3, align 4 //NotApproxable:1
  %11 = load i32, i32* %3, align 4 //NotApproxable:1
  store i32 %11, i32* %2, align 4 //NotApproxable:1
  br label %12
  %13 = load i32, i32* %2, align 4 //NotApproxable:1
  ret i32 %13 //NotApproxable:1
*/
