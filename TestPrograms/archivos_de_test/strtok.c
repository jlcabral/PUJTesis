/* strtok example */
#include <stdio.h>
#include <string.h>

int main ()
{
  char str[] ="imagen1.jpg¿#FTPFSIZE: 15764¿OK¿";
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str);
  pch = strtok (str," ");
  while (pch != NULL)
  {
    printf ("%s\n",pch);
    pch = strtok (NULL,"¿");
  }
  return 0;
}
