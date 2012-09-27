#include <stdio.h>

void swap_temp(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void swap_notemp(int *a, int *b)
{
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}

void swap_xor(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

void driver(int a, int b, void (*func)(int *, int *))
{
	func(&a, &b);
	printf("%d %d\n", a, b);
}

int main(int argc, char *argv[])
{
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);

	driver(a, b, swap_temp);
	driver(a, b, swap_notemp);
	driver(a, b, swap_xor);

	return 0;
}
