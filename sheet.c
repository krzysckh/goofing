#include <varvara.h>
#define USE_FONT
#include "uxn-helpers.c"

#error "not implemented"

#define WIDTH 800
#define HEIGHT 400

#define SHEET_WIDTH 8
#define SHEET_HEIGHT 16

#define INPUTBAR_HEIGHT 32

#define CELL_WIDTH  (WIDTH / SHEET_WIDTH)
#define CELL_HEIGHT ((HEIGHT - INPUTBAR_HEIGHT) / SHEET_HEIGHT)

#define CALC_N_VARIABLES (SHEET_WIDTH * SHEET_HEIGHT)
#define CALC_VARIABLE_NAME_SIZE 8
#define CALC_ERR_FUNCTION error
#define CALC_IMPLEMENTATION
#include "calc.h"

#define cell_at(x, y) (x) / CELL_WIDTH, ((y) - INPUTBAR_HEIGHT) / CELL_HEIGHT

u8 cur_cell[3];

u8 cursor[8] = {
  0b10000000,
  0b11000000,
  0b11100000,
  0b11110000,
  0b11111000,
  0b00100000,
  0b00010000,
  0b00000000,
};

void grid(u8 c) {
  u32 i, j;

  for (i = 0; i < SHEET_HEIGHT; ++i)
    for (j = 0; j < WIDTH; j ++)
      dpixel(j, INPUTBAR_HEIGHT + (i * CELL_HEIGHT), c);

  for (i = 1; i < SHEET_WIDTH; ++i)
    for (j = 0; j < HEIGHT; j++)
      dpixel(i * CELL_WIDTH, INPUTBAR_HEIGHT + j, c);
}

void draw_inputbar(void) {

}

void xytonam(u8 x, u8 y, u8 nam[3]) {
  nam[0] = x + 'a';
  nam[1] = y < 10 ? '0' : '0' + (y / 10);
  nam[2] = y < 10 ? y + '0' : (y % 10) + '0';
}

void highlight_cell(u8 x, u8 y) {
  u32 x1 = x * CELL_WIDTH, y1 = y * CELL_HEIGHT + 1 + INPUTBAR_HEIGHT,
      x2 = x1 + CELL_WIDTH - 1, y2 = y1 + CELL_HEIGHT - 2;
  u8 nam[3];

  xytonam(x, y, nam);
  Vprint(x1 + 2, y1 + 1, nam, 3);

  for (; x1 <= x2; x1++) {
    dpixel(x1, y1, 2);
    dpixel(x1, y2, 2);
  }
}

void on_screen() {
  dpixel(0, 0, BgFillBR);
  dpixel(0, 0, FgFillBR);

  grid(0x01 | Bg1);

  highlight_cell(cell_at(mouse_x(), mouse_y()));

  set_screen_xy(mouse_x(), mouse_y());
  set_screen_addr(cursor);
  draw_sprite(2 | Fg1);
}

void main (void) {
  set_screen_size(WIDTH, HEIGHT);
  set_palette(0xd2d7, 0xd237, 0xd227);
}
