#define SPXE_APPLICATION
#include <spxe.h>
#include <inttypes.h>
#include <math.h>

/* written while waiting for my flight */

#define WIN_WIDTH 400
#define WIN_HEIGHT 300
#define WIDTH WIN_WIDTH
#define HEIGHT WIN_HEIGHT
#define BORDER_SIZE 8
#define _at(w,p,x,y) (p[(((y) * (w)) + (x))])
#define at(x,y) (p[(((y) * (WIDTH)) + (x))])

#define set(x, y, c) { at((x),(y)).r = (c).r; \
                       at((x),(y)).g = (c).g; \
                       at((x),(y)).b = (c).b; \
                       at((x),(y)).a = (c).a; }

/* TODO: replace this with a less-cursed dvd logo */
/* i drew this one myself in gimp B) */
static uint16_t dvd[] = {
  0xffff, 0xffff, 0xffff, 0xffff,
  0xffff, 0xe01f, 0xffff, 0x7,
  0xfffc, 0x7,    0xfff0, 0x3,
  0xffc0, 0x3,    0xff80, 0x3,
  0xff00, 0xe007, 0xfe03, 0xf007,
  0xfc07, 0xf00f, 0xf80f, 0xe00f,
  0xf81f, 0xc01f, 0xf01f, 0x803f,
  0xf00e, 0x7f,   0xe000, 0xff,
  0xe000, 0x1ff,  0xe000, 0x7ff,
  0xf000, 0x1fff, 0xf000, 0x7fff,
  0xfc03, 0xffff, 0xffff, 0xffff,
  0xffff, 0xffff, 0xfe3b, 0xd1ff,
  0xfe1b, 0xd0ff, 0xfe69, 0xd37f,
  0xfe69, 0xd37f, 0xfe6c, 0xb37f,
  0xfe6c, 0x337f, 0xfe0e, 0x307f,
  0xfe1e, 0x70ff, 0xffff, 0xffff,
};
static uint8_t dvd_len = sizeof(dvd) / sizeof(*dvd);

static Px F  = { 0xcc, 0xcc, 0x00, 0xff };
static Px B  = { 0x00, 0x00, 0xee, 0xff };
static Px *p = NULL;

void show_dvd(int x, int y) {
  int i, j, cur, xctr = 0, yctr = 0;
  for (i = 0; i < dvd_len; ++i) {
    for (j = 16; j > 0; --j) {
      cur = (dvd[i] >> (j - 1)) & 1;
      if (!cur)
        set(x + xctr, y - yctr, F);
      xctr = (xctr + 1) % 32, yctr = xctr == 0 ? yctr + 1 : yctr;
    }
  }
}

int main(void) {
  int i, j;
  float x = (float)WIDTH / 2, y = (float)HEIGHT / 2, vx = 0.75, vy = -0.75;

  p = spxeStart("dvd", WIN_WIDTH, WIN_HEIGHT, WIDTH, HEIGHT);

  for (i = 0; i < HEIGHT; ++i)
    for (j = 0; j < WIDTH; ++j)
      set(j, i, ((Px){ 0xee, 0xee, 0xee, 0xff }));

  for (i = BORDER_SIZE; i < HEIGHT - BORDER_SIZE; ++i)
    for (j = BORDER_SIZE; j < WIDTH - BORDER_SIZE; ++j)
      set(j, i, B);

  while (spxeRun(p)) {
    if (spxeKeyPressed(ESCAPE)) break;

    /* clear after the one before */
    for (i = floor(y) - 1; i > floor(y) - 32; --i)
      for (j = floor(x); j < floor(x) + 32; ++j)
        set(j, i, B);

    /* logic */
    if (x + 32 >= WIDTH - BORDER_SIZE)
      vx = -vx;
    if (x <= 0 + BORDER_SIZE)
      vx = -vx;
    if (y >= HEIGHT - BORDER_SIZE)
      vy = -vy;
    if (y - 32 <= 0 + BORDER_SIZE)
      vy = -vy;

    x += vx, y += vy;
    show_dvd(floor(x), floor(y));
  }

  return spxeEnd(p);
}
