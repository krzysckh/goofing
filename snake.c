#include <varvara.h>
#include "uxn-helpers.c"

#define W 128
#define H 128
#define AW (W/8)
#define AH (H/8)

struct pos { u8 x, y, d, n; }; /* x, y, direction, and next direction */
enum       { dN, dE, dS, dW }; /* north, east, south and west */

u8 block[8] = { 0x7e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7e };

struct pos tail[AW * AH], apple;
u32 tailsz = 1;
u8 newtail = 0, timer = 0;

u8 in_snake(u8 x, u8 y, u8 start) {
  u32 i;

  for (i = start; i < tailsz; ++i)
    if (tail[i].x == x && tail[i].y == y)
      return 1;
  return 0;
}

void gen_apple(void) {
  u8 x, y;
R:
  x = rand() % AW;
  y = rand() % AH;

  if (in_snake(x, y, 0)) goto R;

  apple.x = x, apple.y = y;
}

void draw_block(u8 x, u8 y, u8 c) {
  set_screen_addr(block);
  set_screen_xy(x * 8, y * 8);
  draw_sprite(c);
}

void update_pos(struct pos *P) {
  switch (P->d) {
    case dN: P->y--; break;
    case dE: P->x++; break;
    case dS: P->y++; break;
    case dW: P->x--; break;
  }
}

/* TODO: some 'you lost' screen */
void lost() {
  puts("lost");
  exit(1);
}

u8 check_lost() {
  if (tail->x < 0 || tail->x > AW || tail->y < 0 || tail->y > AH) return 1;
  return in_snake(tail->x, tail->y, 1);
}

void on_controller(void) {
  int i;

  switch (controller_button()) {
    case ButtonUp:    tail->d = dN; break;
    case ButtonDown:  tail->d = dS; break;
    case ButtonLeft:  tail->d = dW; break;
    case ButtonRight: tail->d = dE; break;
  }

  for (i = 1; i < tailsz; ++i)
    tail[i].n = tail[i-1].d;
}

void on_screen(void) {
  u32 i;
  if (timer % 15 == 0) {
    set_screen_xy(0, 0);
    draw_pixel(BgFillBR);

    update_pos(&*tail);
    for (i = 1; i < tailsz; ++i)
      update_pos(&tail[i]);

    if (newtail) {
      tailsz++;
      newtail = 0;
    }

    for (i = 1; i < tailsz; ++i) {
      tail[i].d = tail[i].n;
      tail[i].n = tail[i-1].d;
    }

    if (in_snake(apple.x, apple.y, 0)) {
      gen_apple();
      tail[tailsz].x = tail[tailsz - 1].x;
      tail[tailsz].y = tail[tailsz - 1].y;
      tail[tailsz].n = tail[tailsz - 1].d;
      tail[tailsz].d = tail[tailsz - 1].d;

      newtail = 1;
    }

    if (check_lost())
      lost();

    draw_block(tail->x, tail->y, 2);
    for (i = 1; i < tailsz; ++i)
      draw_block(tail[i].x, tail[i].y, 1);
    draw_block(apple.x, apple.y, 3);
  }

  timer = (timer + 1) % 60;
}

void main (void) {
  srand(datetime_second() + datetime_minute() * 60);

  set_palette(0x08ae, 0x0c01, 0x44f1);
  set_screen_size(W, H);

  tail->x = AW / 2;
  tail->y = AH / 2;
  tail->d = dN;
  tail->n = dN;

  gen_apple();
}
