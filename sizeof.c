#include <varvara.h>
#include "uxn-helpers.c"

void main(void)
{
  puts("unsigned char");
  putd(sizeof(unsigned char));
  puts("signed char");
  putd(sizeof(signed char));

  puts("unsigned short int");
  putd(sizeof(unsigned short int));
  puts("unsigned int");
  putd(sizeof(unsigned int));

  puts("signed short int");
  putd(sizeof(signed short int));
  puts("signed int");
  putd(sizeof(signed int));

  exit(0);
}
