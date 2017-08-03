int x = 0;
int a[10];

void foo() {
  a[5] = x + 2;
}

int main(void) {
  foo();
  return a[a[5]];
}

/*
#Function: main
 *Parents:
 *Childs:
   -foo
 *Critical Addresses:
  i32* getelementptr inbounds ([10 x i32], [10 x i32]* @a, i64 0, i64 5) //NotApproxable:0
 *Global Variables:
  @x = global i32 0, align 4 //NotApproxable:0
  @a = common global [10 x i32] zeroinitializer, align 16 
  i32* getelementptr inbounds ([10 x i32], [10 x i32]* @a, i64 0, i64 5) //NotApproxable:0
 *Arguments:
 *Instructions:
  %1 = alloca i32, align 4 
  store i32 0, i32* %1, align 4 
  call void @foo() 
  %2 = load i32, i32* getelementptr inbounds ([10 x i32], [10 x i32]* @a, i64 0, i64 5), align 4 //NotApproxable:1
  %3 = sext i32 %2 to i64 //NotApproxable:1
  %4 = getelementptr inbounds [10 x i32], [10 x i32]* @a, i64 0, i64 %3 //NotApproxable:1
  %5 = load i32, i32* %4, align 4 
  ret i32 %5 

#Function: foo
 *Parents:
   -main
 *Childs:
 *Critical Addresses:
  @x = global i32 0, align 4 //NotApproxable:0
 *Global Variables:
  @x = global i32 0, align 4 //NotApproxable:0
  @a = common global [10 x i32] zeroinitializer, align 16 
  i32* getelementptr inbounds ([10 x i32], [10 x i32]* @a, i64 0, i64 5) //NotApproxable:0
 *Arguments:
 *Instructions:
  %1 = load i32, i32* @x, align 4 //NotApproxable:1
  %2 = add nsw i32 %1, 2 //NotApproxable:1
  store i32 %2, i32* getelementptr inbounds ([10 x i32], [10 x i32]* @a, i64 0, i64 5), align 4 //NotApproxable:1
  ret void 
*/
