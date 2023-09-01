#include "varvara.h"
#include "uxn-helpers.c"

/* i have no clue how it works. i have read none of the varvara docs.
 * this is plain guesswork based on prior c knowledge.
 */

#define pixel_size 8

/* window width and height */
#define WIDTH 400
#define HEIGHT 400

/* actual width and actual height */
#define AW (WIDTH/pixel_size)
#define AH (HEIGHT/pixel_size)

#define BG 1
#define SA 2
#define FG 3

#define at(x,y) (state[(((y) * (AW)) + (x))])

int state[AW * AH];

/* FIXME: i WILL re-do it so it's fast, but i'm too tired rn */
void update_state() {
  int x, y, cur;
  for (y = -1 + AH; y >= 0; --y) {
    for (x = 0; x < AW; ++x) {
      if (at(x, y) == SA) {
        if ((y + 1) < AH) {
          if (at(x, y + 1) == SA) {
            if (x - 1 >= 0 && at(x - 1, y + 1) == BG) {
              at(x, y) = BG;
              at(x - 1, y + 1) = SA;
            } else if (x + 1 < AW && at(x + 1, y + 1) == BG) {
              at(x, y) = BG;
              at(x + 1, y + 1) = SA;
            }
          } else {
            at(x, y) = BG;
            at(x, y + 1) = SA;
          }
        }
      }
    }
  }
}

void P(int x, int y, char c) {
  int dx, dy;

  x = x * pixel_size, y = y * pixel_size;

  for (dy = 0; dy < pixel_size; dy++)
    for (dx = 0; dx < pixel_size; dx++)
      dpixel(x + dx, y + dy, c);
}

void on_mouse(void) {
  if (mouse_state())
    at(mouse_x() / pixel_size, mouse_y() / pixel_size) = SA;
}

void on_screen(void) {
  int i, j;

  update_state();
  for (i = 0; i < AH; ++i)
    for (j = 0; j < AW; ++j)
      P(j, i, at(j, i));
}

void main (void) {
  int i;
  for (i = 0; i < AW * AH; ++i)
    state[i] = BG;

  set_screen_size(WIDTH, HEIGHT);
  set_palette(0x34d0, 0x34d0, 0x4f00);
}
