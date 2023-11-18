#define __uxn__

#define size_t u16
#define INT_MAX 32768
#define INT_MIN -INT_MAX
#define SHRT_MAX 127
#define SHRT_MIN -127

#define uint8_t unsigned char
#define int8_t  signed char
#define uint16_t unsigned short int
#define int16_t signed short int

#define u8 uint8_t
#define u16 uint16_t

#define i8 int8_t
#define i16 int16_t

#define NULL ((void*)0)
#define nil NULL
#define true 1
#define false 0
#define nullptr 1 /* i am sending hate to all the C++ developers */

#define dpixel(x,y,c) { set_screen_xy((x), (y)); draw_pixel((c)); }
#define srandt() srand(datetime_second() + (datetime_minute() * 60))
#define puts(s) pputs(1, 0, s)
#define eputs(s) pputs(1, 1, s)
#define error(s) { pputs(0, 1, "error: "); eputs(s); exit(1); }
#define putd(n) { pputd(n); putchar('\n'); }
#define assert(x) { if (!(x)) { eputs(#x); error("assertion failed."); } }

#define isdigit(d) ((d) >= '0' && (d) <= '9')
#define strcpy(a, b) strncpy(a, b, strlen(b))
#define abs(x) ((x) < 0 ? -(x) : (x))

#ifdef USE_FONT
#ifdef FONT_ATARI
#include "atari8.h"
#define FF atari8
#else
#include "vga-font.h"
#define FF vga_font
#endif


void Vputc(u16 x, u16 y, u8 chr, u8 color) {
  set_screen_addr(FF[chr]);
  set_screen_xy(x, y);
  draw_sprite(color);
}

void Vprint(u16 x, u16 y, u8 *s, u8 color) {
  while (*s) {
    Vputc(x, y, *s, color);
    x += 8, s++;
  }
}
#endif

/* lmao */
u16 _rand_seed_state = 2139;

/* https://en.wikipedia.org/wiki/Xorshift */
/* i had to /2 sume numbers because there's no u32 */
u16 rand() {
  u16 x = _rand_seed_state;
  x ^= x << 3;
  x ^= x >> 7;
  x ^= x << 1;
  return _rand_seed_state = x;
}

void srand(u16 x) {
  _rand_seed_state = x;
}

void memset(u8 *s, u16 c, u16 n) {
  while (n--)
    *s++ = c;
}

void draw_rect(u16 _x1, u16 y1, u16 x2, u16 y2, u8 c) {
  u16 x1 = _x1;
  for (; x1 <= x2; ++x1) {
    dpixel(x1, y1, c);
    dpixel(x1, y2, c);
  }

  x1 = _x1;
  for (; y1 <= y2; ++y1) {
    dpixel(x1, y1, c);
    dpixel(x2, y1, c);
  }
}

u16 strlen(u8 *s) {
  u16 i = 0;
  while (*s)
    i++, s++;

  return i;
}

u8 *strrev(u8 *s) {
  u8 *e = s + strlen(s) - 1, t;

  while (e > s) {
    t = *s, *s = *e, *e = t;
    ++s, --e;
  }

  return s;
}

void itos(i16 n, u8 *buf) {
  i16 a;
  if (n < 0) {
    *buf = '-', buf++;
    n = -n;
  }

  while (n > 9) {
    a = n / 10;
    n -= 10 * a;
    itos(a, buf + 1);
  }

  *buf = '0' + n;
  strrev(buf);
}
#define itoa(n, buf) itos(n, buf)

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

i16 atoi(u8 *_s) {
  i16 r = 0, ctr = 1;
  u8 *s = _s + strlen(_s) - 1;

  while (s >= _s) {
    if (s == _s && *s == '-') return -r;
    r += (ctr) * (*s - '0');
    ctr *= 10, s--;
  }

  return r;
}

void strncpy(u8 *a, u8 *b, u16 n) {
  while ((*a++ = *b++) && n--)
    ;

  *a = 0;
}

void memcpy(u8 *a, u8 *b, u16 n) {
  while ((*a++ = *b++) && n--)
    ;
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

void pputd(i16 n) {
  u16 a;
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

