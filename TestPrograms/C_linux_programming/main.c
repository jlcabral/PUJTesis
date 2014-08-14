#include <stdio.h>

int main(int argc, char *argv[])
{
	char x[3] = {0,1,0};
	char y[2] = {0,1};


	if(!memcmp(x,y,3))
	   printf("funciono!\n");
	else{
	    printf("no funciono\n");
	}

	return 0;
}
