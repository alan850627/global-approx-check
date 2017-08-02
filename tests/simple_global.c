int x = 10;

void foo() {
  x = x + 10;
}

int main(void) {
  foo();
  int y[10];
  return y[x];
}

/*
#Function: main
 *Parents:
 *Childs:
   -foo
 *Critical Addresses:
  @x = global i32 10, align 4 //NotApproxable:0
 *Global Variables:
  @x = global i32 10, align 4 //NotApproxable:0
 *Arguments:
 *Instructions:
  %1 = alloca i32, align 4
  %y = alloca [10 x i32], align 16
  store i32 0, i32* %1, align 4
  call void @foo()
  %2 = load i32, i32* @x, align 4 //NotApproxable:1
  %3 = sext i32 %2 to i64 //NotApproxable:1
  %4 = getelementptr inbounds [10 x i32], [10 x i32]* %y, i64 0, i64 %3 //NotApproxable:1
  %5 = load i32, i32* %4, align 4
  ret i32 %5

#Function: foo
 *Parents:
   -main
 *Childs:
 *Critical Addresses:
  @x = global i32 10, align 4 //NotApproxable:0
 *Global Variables:
  @x = global i32 10, align 4 //NotApproxable:0
 *Arguments:
 *Instructions:
  %1 = load i32, i32* @x, align 4 //NotApproxable:1
  %2 = add nsw i32 %1, 10 //NotApproxable:1
  store i32 %2, i32* @x, align 4 //NotApproxable:1
  ret void
*/
