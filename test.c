void foo2();

void foo(int* x) {
	*x = 1+1; //*
	foo2();
}

int lotsOfInputs(int x, int y, char z, long ll) {
	int a;
	int b;
	return 1;
}

int s[] = {1, 2, 3, 4, 5, 6};
int main() {
	int x = 4; //*
	foo (&x);
	lotsOfInputs(1, x, 3, 4);
	return s[x]; //*
}

void foo2() {
	main();
	foo2();
}
