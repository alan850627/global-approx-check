// Test 1
void foo(int* x) {
	*x = 1+1; //*
}

int main() {
	int x = 4; //*
	char[] s = "hello world"; //*
	foo (&x);
	return s[x]; //*
}

// Test 2
int b(int x) { //*
	int z = 3;
	char[] s = "hello world"
	z = z + s[x]; //*
	return z;
}

int a() {
	int x = 1; //*
	int y = 2; //*
	x = y + 5; //*
	int z = b(x);
	return y; //*
}

// Test 3
int c(int z) { //*
	z++; //*
	return z; //*
}
int b(int x) {  //*
	int z = 3;  //*
	char[] s = "hello world"
	int q = c(z) + s[x];  //*
	return s[q];
}

int a() {
	char[] s = "hello world";
	int x = 1; //*
	int y = 2; //*
	int z = b(x);
	return x + y;
}
