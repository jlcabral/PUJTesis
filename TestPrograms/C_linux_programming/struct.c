#include <stdio.h>

int main(int argc, char *argv[])
{
	struct My_st {
		int a;
		float b;
		int c;
	} /*myst*/;
	typedef struct My_st My_st;

	My_st myst;
	myst.a = 2;
	myst.b = 2.072;
	myst.c = 2;

	printf("a = %d, b = %f, c = %d\n", myst.a, myst.b, myst.c);
	return 0;
}
