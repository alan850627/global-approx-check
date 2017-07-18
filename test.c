// Test 1
void foo(int* x) {
	*x = 1+1; //*
}

int main() {
	int x = 4; //*
	char s[] = "hello world"; //*
	foo (&x);
	return s[x]; //*
}

// Test 2
int b1(int x) { //*
	int z = 3;
	char s[] = "hello world";
	z = z + s[x]; //*
	return z;
}

int a1() {
	int x = 1; //*
	int y = 2; //*
	x = y + 5; //*
	int z = b1(x);
	return y; //*
}

// Test 3
int c2(int z) { //*
	z++; //*
	return z; //*
}
int b2(int x) {  //*
	int z = 3;  //*
	char s[] = "hello world";
	int q = c2(z) + s[x];  //*
	return s[q];
}

int a2() {
	char s[] = "hello world";
	int x = 1; //*
	int y = 2; //*
	int z = b2(x);
	return x + y;
}
