#include <stdio.h>
#include <string.h>

#define START "$$$"
#define TEST_STRING "123456789"

char buf[]="This is simple string";
//size_t strlen (const char *s)

int main(int argc, char *argv[])
{
	int longitud;

	longitud = strlen(START);	
	printf("longitud = %d \n",longitud);
	
	char *pch;

	pch = strstr(buf,"xxx");

	printf("ptr = %d \n",pch);

	return 0;
}
