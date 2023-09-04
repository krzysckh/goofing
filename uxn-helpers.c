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
#ifdef FONT_ATARI
#include "atari8.h"
#define FF atari8
#else
#include "vga-font.h"
#define FF vga_font
#endif


void Vputc(u32 x, u32 y, u8 chr, u8 color) {
  set_screen_addr(FF[chr]);
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

void memset(u8 *s, u32 c, u32 n) {
  while (n--)
    *s++ = c;
}

void draw_rect(u32 _x1, u32 y1, u32 x2, u32 y2, u8 c) {
  u32 x1 = _x1;
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

u32 strlen(u8 *s) {
  u32 i = 0;
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

void itos(i32 n, u8 *buf) {
  i32 a;
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
