#define uint8_t unsigned char
#define uint16_t unsigned short int
#define uint32_t unsigned int
#define int8_t signed char
#define int16_t signed short int
#define int32_t signed int

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define i8 int8_t
#define i16 int16_t
#define i32 int32_t

#define dpixel(x,y,c) { set_screen_xy((x), (y)); draw_pixel((c)); }

/* lmao */
u32 _rand_seed_state = 2139;

/* https://en.wikipedia.org/wiki/Xorshift */
u32 rand() {
	u32 x = _rand_seed_state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return _rand_seed_state = x;
}

void srand(u32 x) {
  _rand_seed_state = x;
}

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

void pputd(u32 n) {
  u32 a;
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

