#include <stdio.h>

int main() {
  int a[] = {1, 2, 3};
  a[1] = a[2] + 226;
  printf("%d", a[1]);
  return 0;
}

/*
#Function: main
 *Parents:
 *Childs:
   -printf
 *Critical Addresses:
  %7 = getelementptr inbounds [3 x i32], [3 x i32]* %a, i64 0, i64 1 //NotApproxable:1
  %3 = getelementptr inbounds [3 x i32], [3 x i32]* %a, i64 0, i64 2 //NotApproxable:1
 *Global Variables:
  @main.a = private unnamed_addr constant [3 x i32] [i32 1, i32 2, i32 3], align 4 
  @.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1 
 *Arguments:
 *Instructions:
  %1 = alloca i32, align 4 
  %a = alloca [3 x i32], align 4 
  store i32 0, i32* %1, align 4 
  %2 = bitcast [3 x i32]* %a to i8* //NotApproxable:1
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* bitcast ([3 x i32]* @main.a to i8*), i64 12, i32 4, i1 false) //NotApproxable:1
  %3 = getelementptr inbounds [3 x i32], [3 x i32]* %a, i64 0, i64 2 //NotApproxable:1
  %4 = load i32, i32* %3, align 4 //NotApproxable:1
  %5 = add nsw i32 %4, 226 //NotApproxable:1
  %6 = getelementptr inbounds [3 x i32], [3 x i32]* %a, i64 0, i64 1 //NotApproxable:1
  store i32 %5, i32* %6, align 4 //NotApproxable:1
  %7 = getelementptr inbounds [3 x i32], [3 x i32]* %a, i64 0, i64 1 //NotApproxable:1
  %8 = load i32, i32* %7, align 4 //NotApproxable:1
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 %8) //NotApproxable:1
  ret i32 0 

#Function: printf
 *Parents:
 *Childs:
 *Critical Addresses:
 *Global Variables:
  @main.a = private unnamed_addr constant [3 x i32] [i32 1, i32 2, i32 3], align 4 
  @.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1 
 *Arguments:
 *Instructions:
*/
