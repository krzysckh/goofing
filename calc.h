/* simple stack-based calculator for integer calculations */

signed int calc(char *s);
void calc_set(char *name, signed int value);
signed int calc_get(char *name);

#ifndef CALC_STACK_SIZE
#define CALC_STACK_SIZE 1024
#endif

#ifndef CALC_N_VARIABLES
#define CALC_N_VARIABLES 1024
#endif

#ifndef CALC_VARIABLE_NAME_SIZE
#define CALC_VARIABLE_NAME_SIZE 16
#endif

#ifndef CALC_ERR_FUNCTION
#define calcerr(s) fprintf(stderr, "calc error: %s\n", s)
#define CALC_ERR_FUNCTION calcerr
#endif

#ifdef CALC_IMPLEMENTATION

struct variable_t {
  char name[CALC_VARIABLE_NAME_SIZE];
  signed int v;
};

static signed int calc_stack[CALC_STACK_SIZE];
static signed int calc_stack_ptr = 0;

static struct variable_t calc_variable[CALC_N_VARIABLES];
static signed int calc_variable_ptr = 0;

#define calc_peek() (calc_stack_ptr ? calc_stack[calc_stack_ptr - 1] : 0)

void calc_set(char *name, signed int value) {
  signed int i;

  for (i = 0; i < calc_variable_ptr; ++i) {
    if (strcmp(calc_variable[i].name, name) == 0) {
      calc_variable[i].v = value;
      return;
    }
  }

  if (calc_variable_ptr > CALC_N_VARIABLES)
    CALC_ERR_FUNCTION("out of memory. bump CALC_N_VARIABLES");

  strncpy(calc_variable[calc_variable_ptr].name, name,
      strlen(name) > CALC_VARIABLE_NAME_SIZE ? CALC_VARIABLE_NAME_SIZE - 1
      : strlen(name));
  calc_variable[calc_variable_ptr].v = value;
  calc_variable_ptr++;
}

signed int calc_get(char *name) {
  signed int i;

  for (i = 0; i < calc_variable_ptr; ++i)
    if (strcmp(calc_variable[i].name, name) == 0)
      return calc_variable[i].v;

  CALC_ERR_FUNCTION(name);
  CALC_ERR_FUNCTION("no such variable");
  return 0;
}

static signed int calc_pop() {
  if (calc_stack_ptr)
    return calc_stack[--calc_stack_ptr];
  else
    CALC_ERR_FUNCTION("calc_pop(): no values on stack");

  return 0;
}

static void calc_push(signed int v) {
  if (calc_stack_ptr > CALC_STACK_SIZE) {
    CALC_ERR_FUNCTION("calc_push(): stack overflow");
    return;
  }

  calc_stack[calc_stack_ptr++] = v;
}

static void dispatchs(char *s) {
  if (strlen(s) == 1) {
    switch (*s) {
      case '+': calc_push(calc_pop() + calc_pop()); break;
      case '-': calc_push(calc_pop() - calc_pop()); break;
      case '/': calc_push(calc_pop() / calc_pop()); break;
      case '*': calc_push(calc_pop() * calc_pop()); break;
      case '=': calc_push(calc_pop() == calc_pop()); break;
      default: goto F;
    }
  } else {
F:
    calc_push(calc_get(s));
  }
}

signed int calc(char *s) {
  char buf[64] = { 0 };
  int cur_buf = 0;

  while (1) {
    if (*s == ' ' || *s == 0) {
      buf[cur_buf] = 0;

      if (isdigit(*buf) || (*buf == '-' && isdigit(*(buf + 1))))
        calc_push(atoi(buf));
      else
        dispatchs(buf);

      if (!*s) break;

      cur_buf = 0, s++;
    } else {
      buf[cur_buf++] = *s++;
    }
  }

  return calc_peek();
}

#endif
