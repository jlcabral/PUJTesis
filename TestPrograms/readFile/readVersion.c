#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE_FILE 4

int main()
{
    FILE *ptr_file;
	char bufVersion[MAX_SIZE_FILE];
    char version = 0;
	
	ptr_file = fopen("version","r");
	if (!ptr_file)
    	return 1;
    
	printf("[%s]\n",fgets(bufVersion,MAX_SIZE_FILE, ptr_file) );
	version = atoi(bufVersion);
    printf("[%i]\n",atoi(bufVersion) );
    
	fclose(ptr_file);
	
	return 0;
}
