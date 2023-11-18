#include <varvara.h>

#include "uxn-helpers.c"
#include "line.h"

#define W 256
#define H W

void main(void) {
  u16 i, x1 = 0, y1 = 0, x2 = 170, y2 = H;

  set_screen_size(W, H);
  set_palette(0x0fff, 0x0fff, 0x0fff);

  for (i = 0; i < 256; ++i) {
    line(x1, y1, x2, y2);

    x1 = x2, y1 = y2;
    y2 += (i & 1) ? (y2 / 2) : -(y2 / 2);
    x2 -= (i & 1) ? (x2 / 2) : -(x2 / 2);
  }
}
