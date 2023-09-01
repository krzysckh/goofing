#define dpixel(x,y,c) { set_screen_xy((x), (y)); draw_pixel((c)); }

/* functions starting with an underscore cause a stack overflow.
 * my guess is that they are compiled to the same symbols as the ones without it
 * _puts == puts
 *
 * i guess
 */

void pputs(int nl, int err, char *s) {
  while (*s) {
    if (err)
      console_error(*s);
    else
      putchar(*s);

    s++;
  }

  if (nl) {
    putchar('\n');
  }
}

void puts(char *s) {
  pputs(1, 0, s);
}

void eputs(char *s) {
  pputs(1, 1, s);
}

void error(char *s) {
  pputs(0, 1, "error: ");
  eputs(s);
}

void pputd(int n) {
  int a;
  if (n > 9) {
    a = n / 10;
    n -= 10 * a;
    pputd(a);
  }

  putchar('0' + n);
}

void putd(int n) {
  pputd(n);
  putchar('\n');
}

