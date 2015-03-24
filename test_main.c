#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "pnmio.h" 



/* This code shall be used ONLY for the JPL-UIUC acedemic competition */ 

int main( )
{
  int cols, rows;
  unsigned char *img= NULL;
  img = pgmReadFile("test.pgm", img, &cols, &rows);
  pgmWriteFile("test1.pgm", img, cols, rows );
  
  return 1;
}
