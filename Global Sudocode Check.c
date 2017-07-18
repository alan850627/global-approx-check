int data[];

int B1(int x) { //
	data[x] = 1;
	return x + 1; //
}

int B2(int x) { //
	data[x] = 2;
	return x + 1; //
}

int B3(int x) { //
	data[x] = 3;
	return x + 1; //
}

int B4(int x) { //
	data[x] = 4;
	return x + 1;
}

int A1(int x) { //
	x = x + 1; //
	int y = B1(x); 
	y = y + 1; //
	int z = B2(y);	
	return z; //
}

int A2(int x) { //
	x = x + 1; //
	int y = B3(x);
	y = y + 1; //
	int z = B4(y);	
	return z;
}

int main(int x) { //
	x = x + 1; //
	int y = A1(x); 
	y = y + 1; //
	int z = A2(y);	
}

// 1. main - everything good.
// 2. Go into A1. 
// 3. A1 - everything good.
// 4. Go into B1.
// 5. x used as address - B1 input cannot be approx.
// 6. Line 24 marked.
// 7. A1 input cannot be approx.
// 8. Go into B2
// 9. 