#include <varvara.h>
#include "uxn-helpers.c"

#warning unfinished (no way to win)

#define W 64     /* window sizes */
#define H 96
#define bw 8     /* board sizes */
#define bh 8
#define bombp 25 /* ~25% of the board will be bombs */

#define _at(a, x, y)((a)[((y) * bw) + x])
#define at(x, y) _at(state, x, y)
#define bat(x, y) _at(nbord, x, y)

enum {
  tnone = 1<<1,
  tbomb = 1<<2,
  tmark = 1<<3,
  tshow = 1<<4
};

u8 state[bw * bh];
i8 nbord[bw * bh];

u8 lost = 0, shocked = 0, won = 0;
u32 good = 0, nbombs = 0, picks = 0;

u8 block[8]   = { 0x7e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7e };
u8 sad[8]     = { 0xff, 0x81, 0xa5, 0x81, 0x99, 0xa5, 0x81, 0xff };
u8 happy[8]   = { 0xff, 0x81, 0xa5, 0x81, 0xa5, 0x99, 0x81, 0xff };
u8 shock[8]   = { 0xff, 0x81, 0xa5, 0x81, 0x99, 0xa5, 0x99, 0xff };
u8 glasses[8] = { 0xff, 0x81, 0xff, 0xff, 0x81, 0xa5, 0x99, 0xff };
u8 N[9][8]    = {
  { 0, 0,    0,    0x18, 0x18, 0,    0,    0, },
  { 0, 0x10, 0x30, 0x10, 0x10, 0x10, 0x10, 0, },
  { 0, 0x38, 0x44, 0x48, 0x10, 0x20, 0x7c, 0, },
  { 0, 0x38, 0x44, 0x8,  0x4,  0x44, 0x38, 0, },
  { 0, 0x8,  0x10, 0x28, 0x78, 0x8,  0x8,  0, },
  { 0, 0x7c, 0x40, 0x7c, 0x4,  0x44, 0x38, 0, },
  { 0, 0xc,  0x10, 0x20, 0x78, 0x44, 0x38, 0, },
  { 0, 0x7e, 0x4,  0x4,  0x8,  0x10, 0x10, 0, },
  { 0, 0x38, 0x44, 0x38, 0x44, 0x44, 0x38, 0, }
};

void grid(void) {
  u16 i, j;

  for (i = (H - W) + 8; i < H; i += 8)
    for (j = 0; j < W; j += 2)
      dpixel(j - 1, i, 2);

  for (j = 0; j < W; j += 8)
    for (i = (H - W); i < H; i += 2)
      dpixel(j - 1, i, 2);
}

void lose(void) {
  u8 i, j;
  lost = 1;

  for (i = 0; i < bh; ++i)
    for (j = 0; j < bw; ++j)
      at(j, i) = at(j, i) | tshow;
}

void win(void) {
  set_screen_xy((W/2) - 4, 8);
  set_screen_addr(glasses);
  draw_sprite(14);

  won = 1;
}

void show_near(i8 x, i8 y) {
  i8 dx, dy;
  if (bat(x, y) != 0) return;

  at(x, y) = at(x, y) | tshow;
  for (dy = -1; dy <= 1; ++dy) {
    for (dx = -1; dx <= 1; ++dx) {
      if (dx == 0 && dy == 0) continue;
      if (x + dx >= bw) continue;
      if (x + dx < 0) continue;
      if (y + dy >= bh) continue;
      if (y + dy < 0) continue;

      if (!(at(x + dx, y + dy) & tshow))
        show_near(x + dx, y + dy);
    }
  }
}

void on_mouse(void) {
  u8 rx, ry;

  rx = mouse_x() / 8;
  ry = (mouse_y() - (H - W)) / 8;

  shocked = 0;
  if (mouse_state() & 1)
    if (mouse_x() > (W/2) - 4 && mouse_x() < (W/2) + 4 && mouse_y() > 8
        && mouse_y() < 16)
      shocked = 1;

  /* check for clicking @ the board */
  if (lost || won) return;

  if (mouse_state() & 4) {
    if (mouse_y() < (H - W))
      return;

    if (at(rx, ry) & tmark) {
      at(rx, ry) = at(rx, ry) ^ tmark;
      picks--;
      if (at(rx, ry) & tbomb)
        good--;
    } else {
      at(rx, ry) = at(rx, ry) | tmark;
      picks++;
      if (at(rx, ry) & tbomb)
        good++;
    }
  }
  if (mouse_state() & 1) {
    if (mouse_y() < (H - W))
      return;

    at(rx, ry) = at(rx, ry) | tshow;
    show_near(rx, ry);
    if (at(rx, ry) & tbomb)
      lose();
  }

}

void on_screen(void) {
  u32 i, j;

  if (good == nbombs && good == picks) {
    win();
    return;
  }

  set_screen_xy(0, 0);
  draw_pixel(BgFillBR);

  set_screen_xy((W/2) - 4, 8);
  if (lost)
    set_screen_addr(sad);
  else
    set_screen_addr(shocked ? shock : happy);
  draw_sprite(14);

  for (i = 0; i < 8; ++i) {
    for (j = 0; j < 8; ++j) {
      set_screen_addr(block);
      set_screen_xy(j * 8, H - (bh * 8) + (i * 8));
      if (at(j, i) & tbomb && at(j, i) & tshow)
        draw_sprite(2);
      else if (at(j, i) & tmark)
        draw_sprite(1);
      else if (at(j, i) & tshow) {
        set_screen_addr(N[bat(j, i)]);
        draw_sprite(2);
      } else {
        draw_sprite(0);
      }
    }
  }

  grid();

  dpixel(mouse_x(), mouse_y(), 3);
  dpixel(mouse_x(), mouse_y() + 1, 3);
  dpixel(mouse_x() + 1, mouse_y() + 1, 3);
}

void main(void) {
  i16 i, j, dx, dy;

  srandt();
  set_screen_size(W, H);
  set_palette(0xbf0f, 0xb00f, 0xb000); /* gray, red, black, yellow */

  /* init states */
  for (i = 0; i < bw * bh; ++i)
    if (rand() % 100 < bombp) {
      state[i] = tbomb;
      nbombs++;
    } else
      state[i] = tnone;

  for (i = 0; i < bh; ++i) {
    for (j = 0; j < bw; ++j) {
      if (at(j, i) & tbomb) {
        bat(j, i) = -1;
        continue;
      }
      bat(j, i) = 0;
      for (dy = -1; dy <= 1; ++dy) {
        for (dx = -1; dx <= 1; ++dx) {
          if (dx == 0 && dy == 0) continue;
          if (j + dx >= bw) continue;
          if (j + dx < 0) continue;
          if (i + dy >= bh) continue;
          if (i + dy < 0) continue;

          if (at(j + dx, i + dy) & tbomb)
            bat(j, i)++;
        }
      }
    }
  }
}
