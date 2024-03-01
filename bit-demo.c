#include <varvara.h>
#include "uxn-helpers.c"

u16 W = 128;
u16 H = 128;

u8 speed = 64;

u8
f(u16 x, u16 y)
{
  return (x ^ y) % 9;
}

void
on_controller()
{
  set_screen_xy(0, 0);

  switch (controller_key()) {
  case 'q': exit(0); break;
  case 'r':
    draw_pixel(FgFillBR);
    draw_pixel(BgFillBR);
    break;
  }
}

void
on_screen(void)
{
  u16 x, y, i;

  for (i = 0; i < speed; ++i) {
    x = rand() % W, y = rand() % H;
    dpixel(x, y, f(x, y) ? 0 : 1);
  }
}

void
main(void)
{
  srand(32);
  set_palette(0x0fff, 0x0fff, 0x0fff);
  set_screen_size(W, H);

  set_screen_xy(0, 0);
  draw_pixel(FgFillBR);
  draw_pixel(BgFillBR);
}
