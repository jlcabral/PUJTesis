/*
Este programa se realizó para realizar el mapeo de memoria de
un archivo de texto y especialmente una imagen, se apoyó en los
siguientes enlaces y el programa "xxd"

http://es.wikipedia.org/wiki/Archivo_proyectado_en_memoria
http://www.youtube.com/watch?v=FqmqNSa4Xh0
http://linux.die.net/man/2/mmap

el pmap es un apuntador (que posee todas las características aritméticas),
para el manejo de strings se pueden usar funciones como 
strncpy(pmap, "$", 1);

Se compila gcc -o salida binary_info.c

Mucho cuidado al recorrer el archivo, recordar el overflow.

Se logró hacer el paso del mapeo del archivo de modo correcto.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, char *argcv[])
{
	unsigned char buf;
	int i;
	int fd;
	int fd_new;
	struct stat mystat;
	void *pmap;
	char *pointer;
	
	FILE *ptr_file;

	fd = open("archivo.pdf", O_RDWR);
	
	ptr_file =fopen("output.pdf","w");
	//buf=0x02;
	//fputs(&buf, ptr_file);	

	close(fd_new);
	if(fd == -1)
	{
		perror("open");
		exit(1);
	}

	if( fstat(fd, &mystat) < 0)
	{
		perror("fstat");
		close(fd);
		exit(1);
	}

	pmap = mmap(0, mystat.st_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE, fd, 0);

	if( pmap == MAP_FAILED)
	{
		perror("mmap");
		close(fd);
		exit(1);
	}
	pointer = (char *)(pmap);
	printf("size = %i\n",(int)(mystat.st_size));
	
	for(i=0;i<mystat.st_size;i++)
	{				
		//fputs(pointer, ptr_file);
		fwrite(pointer,1,1,ptr_file);
		pointer++;
	}

	//pointer += 0xd00;
	printf("pmap = %hhx\n",(*pointer));
	//strncpy(pmap, "$", 1);
	close(fd);
	return 0;
}