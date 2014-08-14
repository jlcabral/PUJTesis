#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

int file_exist (char *filename)
{
  struct stat buffer;   
  return (stat(filename, &buffer) == 0);
};

int main(){

    printf("hello world\n");
    if (file_exist ("myfile.txt")){
      printf ("It exists\n");
    }else{
        printf("No hay archivo\n");
    }
}
