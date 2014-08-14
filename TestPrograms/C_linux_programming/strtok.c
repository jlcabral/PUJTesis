/* strtok example */
#include <stdio.h>
#include <string.h>

int main ()
{
  char str[] ="$This$a$sample$string.";
  char * pch;
  char * savePtr;
  printf ("Splitting string \"%s\" into tokens:\n",str);
  pch = strtok_r (str,"$",&savePtr);
  //while (pch != NULL)
  while( pch = strtok_r (NULL,"$",&savePtr) )
  {
    printf ("%s\n",pch);
    //pch = strtok_r (NULL, "$",&savePtr);
  }
  return 0;
}
