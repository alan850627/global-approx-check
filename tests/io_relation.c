

int foo(int x) {
  return x + 123;
}

int main(void) {
  int a[] = {1, 2, 3, 4};
  int x = 10;
  int y = foo(x);
  return a[y];
}

/*
#Function: main
 *Parents:
 *Childs:
   -foo
 *Critical Addresses:
  %4 = alloca i32, align 4 //NotApproxable:0
  %3 = alloca i32, align 4 //NotApproxable:0
 *Global Variables:
 *Arguments:
 *Instructions:
  %1 = alloca i32, align 4
  %2 = alloca [4 x i32], align 16
  %3 = alloca i32, align 4 //NotApproxable:0
  %4 = alloca i32, align 4 //NotApproxable:0
  store i32 0, i32* %1, align 4
  %5 = bitcast [4 x i32]* %2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %5, i8* bitcast ([4 x i32]* @main.a to i8*), i64 16, i32 16, i1 false)
  store i32 10, i32* %3, align 4 //NotApproxable:1
  %6 = load i32, i32* %3, align 4 //NotApproxable:1
  %7 = call i32 @foo(i32 %6) //NotApproxable:0
  store i32 %7, i32* %4, align 4 //NotApproxable:1
  %8 = load i32, i32* %4, align 4 //NotApproxable:1
  %9 = sext i32 %8 to i64 //NotApproxable:1
  %10 = getelementptr inbounds [4 x i32], [4 x i32]* %2, i64 0, i64 %9 //NotApproxable:1
  %11 = load i32, i32* %10, align 4
  ret i32 %11

#Function: foo
 *Parents:
   -main
 *Childs:
 *Critical Addresses:
  %2 = alloca i32, align 4 //NotApproxable:1
 *Global Variables:
 *Arguments:
  %2 = alloca i32, align 4 //NotApproxable:1
 *Instructions:
  store i32 %0, i32* %2, align 4 //NotApproxable:1
  %3 = load i32, i32* %2, align 4 //NotApproxable:1
  %4 = add nsw i32 %3, 123 //NotApproxable:1
  ret i32 %4 //NotApproxable:1
*/
