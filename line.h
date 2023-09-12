#ifdef __uxn__
# ifndef LINE_PLOT
#define _LINE_PLOT(x, y) dpixel(x, y, 1)
#define LINE_PLOT _LINE_PLOT
#endif
#endif

#ifndef LINE_PLOT
#error LINE_PLOT(x, y) not defined
#endif

/* TODO: colors */

/* 1:1 https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#All_cases */
void plot_low(unsigned x0, unsigned y0, unsigned x1, unsigned y1);
void plot_high(unsigned x0, unsigned y0, unsigned x1, unsigned y1);
void line(unsigned x0, unsigned y0, unsigned x1, unsigned y1);

/* compile with implementation by default */
#ifndef LINE_NO_IMPLEMENTATION

void plot_low(unsigned x0, unsigned y0, unsigned x1, unsigned y1) {
  signed dx = x1 - x0,
         dy = y1 - y0,
         yi = 1, D, x, y;

  if (dy < 0) {
    yi = -1;
    dy = -dy;
  }

  D = (2 * dy) - dx;
  y = y0;

  for (x = x0; x < x1; x++) {
    LINE_PLOT(x, y);

    if (D > 0) {
      y = y + yi;
      D = D + (2 * (dy - dx));
    } else
      D = D + 2 * dy;
  }
}



void plot_high(unsigned x0, unsigned y0, unsigned x1, unsigned y1) {
  signed dx = x1 - x0,
         dy = y1 - y0,
         xi = 1, D, x, y;

  if (dx < 0) {
    xi = -1;
    dx = -dx;
  }

  D = (2 * dx) - dy;
  x = x0;

  for (y = y0; y < y1; ++y) {
    LINE_PLOT(x, y);

    if (D > 0) {
      x = x + xi;
      D = D + (2 * (dx - dy));
    } else
      D = D + 2 * dx;
  }
}

void line(unsigned x0, unsigned y0, unsigned x1, unsigned y1) {
  if (abs(y1 - y0) < abs(x1 - x0)) {
    if (x0 > x1)
      plot_low(x1, y1, x0, y0);
    else
      plot_low(x0, y0, x1, y1);
  } else {
    if (y0 > y1)
      plot_high(x1, y1, x0, y0);
    else
      plot_high(x0, y0, x1, y1);
  }
}

#endif /* LINE_NO_IMPLEMENTATION */
