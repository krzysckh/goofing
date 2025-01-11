#include <varvara.h>
#include "uxn-helpers.c"

#define CALC_ERR_FUNCTION eputs
#define CALC_N_VARIABLES 512
#define CALC_VARIABLE_NAME_SIZE 8
#define CALC_IMPLEMENTATION
#include "calc.h"

void
print_stack(void)
{
  int i;
  puts("stack = [");
  for (i = 0; i < calc_stack_ptr; ++i)
    putd(calc_stack[i]);
  puts("]\n");
}

void
main(void)
{
  u8 buf[512]; //temp[512], nam[64], val[64];
  i16 ret;
  u16 read;

  while (1) {
    print_stack();
    puts("> ");
    read = gets(buf, 512);
    /*
    if (strcmp(buf, "pop") == 0)
      ret = calc_pop();
    else if (strncmp(buf, "set ", 4) == 0) {
      temp = strdup(buf+4);
      nam = strdup(strsep(&temp, " "));
      val = strdup(strsep(&temp, " "));

      calc_set(nam, atoi(val));
      free(temp);
      free(nam);
      free(val);
      ret = 0;
    } else
    */
    ret = calc((void*)buf);

    nputs("  ");
    putd(ret);
  }
}
