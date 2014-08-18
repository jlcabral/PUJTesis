#include<time.h>
#include<stdlib.h>
#include<stdio.h>
int main(){
	srand(time(NULL));
	long r = rand() % 16777215;
	//printf("[%li]:[%i]:[%i]:[%i]\n",r,sizeof(int),sizeof(long),sizeof(float));
	printf("[%li]",r);
}
