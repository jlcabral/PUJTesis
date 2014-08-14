/* strtok example */
#include <stdio.h>
#include <string.h>

int main ()
{
  char str[] =",969\r\r\n> ";
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str);
  pch = strtok (str,"\n\r");
  while (pch != NULL)
  {
    printf ("[%s]\n",pch);
    pch = strtok (NULL,"\r\n");
  }
  return 0;
}
