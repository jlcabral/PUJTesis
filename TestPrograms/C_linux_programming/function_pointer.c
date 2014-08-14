#include <stdio.h>

int add_numbers(int a, int b)
{
	int sum;
	sum = a + b;
	return sum;
}

int main(int argc, char *argv[])
{
	int result;
	int (*myfunct_ptr)(int, int);
	myfunct_ptr = &add_numbers;
	result = myfunct_ptr(3,7);
	printf("result = %d\n", result);
	return 0;
}
