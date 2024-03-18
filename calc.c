#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define CALC_N_VARIABLES 512
#define CALC_VARIABLE_NAME_SIZE 8
#define CALC_IMPLEMENTATION
#include "calc.h"

void
print_stack(void)
{
  int i;
  printf("stack = [");
  for (i = 0; i < calc_stack_ptr; ++i)
    printf("%u, ", calc_stack[i]);
  printf("]\n");
}

int
main(void)
{
  char buf[1024];
  char *temp, *nam, *val;
  signed int ret;

  while (!feof(stdin)) {
    print_stack();
    printf("> ");
    fgets(buf, 1024, stdin);
    buf[strlen(buf)-1] = 0;
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
      ret = calc(buf);

    printf("  %u\n", ret);
  }

  return 0;
}
