#warning unfinished

#include <varvara.h>
#define USE_FONT
#include "uxn-helpers.c"

#define WIDTH 800
#define HEIGHT 400

#define SHEET_WIDTH 8
#define SHEET_HEIGHT 16

#define INPUTBAR_PADDING 8
#define INPUTBAR_HEIGHT 32

#define CELL_WIDTH  (WIDTH / SHEET_WIDTH)
#define CELL_HEIGHT ((HEIGHT - INPUTBAR_HEIGHT) / SHEET_HEIGHT)

#define CUR_INPUT_SZ 512

#define CALC_N_VARIABLES (SHEET_WIDTH * SHEET_HEIGHT)
#define CALC_VARIABLE_NAME_SIZE 8
#define CALC_ERR_FUNCTION eputs
#define CALC_IMPLEMENTATION
#include "calc.h"

#define cell_at(x, y) (x) / CELL_WIDTH, ((y) - INPUTBAR_HEIGHT) / CELL_HEIGHT
#define cell_print(x, y, s) Vprint((x * CELL_WIDTH) + 2, \
                                   (y * CELL_HEIGHT) + 2 + INPUTBAR_HEIGHT, \
                                   s, 1);

#define cur_values() (cur_cell[0] - 'a'), (atoi(cur_cell + 1 == '0' ? \
                                                cur_cell + 2 : cur_cell + 1))

i32 cache[SHEET_WIDTH * SHEET_HEIGHT];
u8 cur_cell[4] = { 0 };
u32 cur_input_ptr = 0;
u8 cur_input[CUR_INPUT_SZ] = { 0 };

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

void xytonam(u8 x, u8 y, u8 nam[4]) {
  nam[0] = x + 'a';
  nam[1] = y < 10 ? '0' : '0' + (y / 10);
  nam[2] = y < 10 ? y + '0' : (y % 10) + '0';
  nam[3] = 0;
}

void load_cache(void) {
  u32 i, j;
  u8 nam[4];

  for (i = 0; i < SHEET_HEIGHT; ++i) {
    for (j = 0; j < SHEET_WIDTH; ++j) {
      memset(nam, 0, 4);
      xytonam(j, i, nam);
      cache[(i * SHEET_HEIGHT) + j] = calc_get(nam);
    }
  }
}

void highlight_cell(u8 x, u8 y) {
  u32 x1 = x * CELL_WIDTH, y1 = y * CELL_HEIGHT + 1 + INPUTBAR_HEIGHT;
  u8 nam[4];

  xytonam(x, y, nam);
  Vprint(x1 + CELL_WIDTH - (3 * 8), y1 + CELL_HEIGHT - 9, nam, 3);
}

void select_cell(u8 x, u8 y) {
  draw_rect(x * CELL_WIDTH + 1, (y * CELL_HEIGHT) + INPUTBAR_HEIGHT + 1,
            CELL_WIDTH + x * CELL_WIDTH - 1,
            CELL_HEIGHT + (y * CELL_HEIGHT) + INPUTBAR_HEIGHT - 1, 2);
}


void draw_inputbar(void) {
  draw_rect(INPUTBAR_PADDING, INPUTBAR_PADDING, WIDTH - INPUTBAR_PADDING,
      INPUTBAR_HEIGHT - INPUTBAR_PADDING, 1);
  Vprint(INPUTBAR_PADDING + 2, INPUTBAR_PADDING + 2, cur_input, 1);
}

void eval_cur(void) {
  calc_set(cur_cell, calc(cur_input));
  memset(cur_input, 0, CUR_INPUT_SZ);
  cur_input_ptr = 0;

  load_cache();
}

void on_controller(void) {
  u8 k = controller_key();

  switch (k) {
    case 27: /* esc */
      exit(0);
      break;
    case 8: /* bs */
      if (!*cur_cell) return;
      cur_input[cur_input_ptr = cur_input_ptr ? cur_input_ptr - 1
        : cur_input_ptr]= 0;
      break;
    case 13: /* enter */
      eval_cur();
      break;
    default:
      if (!*cur_cell) return; /* no cell selected */
      cur_input[cur_input_ptr++] = k;
      cur_input[cur_input_ptr] = 0;
      break;
  }
}

void on_mouse() {
  if (mouse_state() == 1)
    if (mouse_y() > INPUTBAR_HEIGHT)
      xytonam(cell_at(mouse_x(), mouse_y()), cur_cell);
}

void on_screen() {
  u8 buf[64];
  i32 i, j;

  dpixel(0, 0, BgFillBR);
  dpixel(0, 0, FgFillBR);

  grid(0x01 | Bg1);

  highlight_cell(cell_at(mouse_x(), mouse_y()));
  draw_inputbar();

  for (i = 0; i < SHEET_HEIGHT; ++i) {
    for (j = 0; j < SHEET_WIDTH; ++j) {
      memset(buf, 0, 64);
      itos(cache[(i * SHEET_HEIGHT) + j], buf);
      cell_print(j, i, buf);
    }
  }

  if (*cur_cell) {
    select_cell(cur_values());
  }

  set_screen_xy(mouse_x(), mouse_y());
  set_screen_addr(cursor);
  draw_sprite(2 | Fg1);
}

void main(void) {
  u32 i, j;
  u8 nam[4];

  set_screen_size(WIDTH, HEIGHT);
  set_palette(0xd2d7, 0xd237, 0xd227);

  /* init calc variables for sheet */
  for (i = 0; i < SHEET_HEIGHT; ++i) {
    for (j = 0; j < SHEET_WIDTH; ++j) {
      xytonam(j, i, nam);
      calc_set(nam, 0);
    }
  }

  load_cache();
}
