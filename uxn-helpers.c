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
#define srandt() srand(datetime_second() + (datetime_minute() * 60))
#define puts(s) pputs(1, 0, s)
#define eputs(s) pputs(1, 1, s)
#define error(s) { pputs(0, 1, "error: "); eputs(s); exit(1); }
#define putd(n) { pputd(n); putchar('\n'); }

#define isdigit(d) ((d) >= '0' && (d) <= '9')
#define strcpy(a, b) strncpy(a, b, strlen(b))

#ifdef USE_FONT
#include "font.h"

void Vputc(u32 x, u32 y, u8 chr, u8 color) {
  set_screen_addr(font[chr]);
  set_screen_xy(x, y);
  draw_sprite(color);
}

void Vprint(u32 x, u32 y, u8 *s, u8 color) {
  while (*s) {
    Vputc(x, y, *s, color);
    x += 8, s++;
  }
}
#endif

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

u32 strlen(u8 *s) {
  u32 i = 0;
  while (*s)
    i++, s++;

  return i;
}

/* non-standard. just compares strings */
u8 strcmp(u8 *s1, u8 *s2) {
  if (strlen(s1) != strlen(s2)) return 1;

  while (*s1)
    if (*s1 != *s2)
      return 1;
    else
      s1++, s2++;

  return 0;
}

i32 atoi(u8 *_s) {
  i32 r = 0, ctr = 1;
  u8 *s = _s + strlen(_s) - 1;

  while (s >= _s) {
    if (s == _s && *s == '-') return -r;
    r += (ctr) * (*s - '0');
    ctr *= 10, s--;
  }

  return r;
}

void strncpy(u8 *a, u8 *b, u32 n) {
  while ((*a++ = *b++) && n--)
    ;

  *a = 0;
}

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

void pputd(i32 n) {
  u32 a;
  if (n < 0) {
    putchar('-');
    n = -n;
  }

  if (n > 9) {
    a = n / 10;
    n -= 10 * a;
    pputd(a);
  }

  putchar('0' + n);
}
