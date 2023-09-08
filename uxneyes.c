#include <varvara.h>
#include "uxn-helpers.c"

/*
 * GLWT(Good Luck With That) Public License
 * Copyright (c) Everyone, except Author
 *
 * Everyone is permitted to copy, distribute, modify, merge, sell, publish,
 * sublicense or whatever they want with this software but at their OWN RISK.
 *
 * Preamble
 *
 * The author has absolutely no clue what the code in this project does. It
 * might just work or not, there is no third option.
 *
 * GOOD LUCK WITH THAT PUBLIC LICENSE
 * TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION, AND MODIFICATION
 *
 *   0. You just DO WHATEVER YOU WANT TO as long as you NEVER LEAVE A TRACE TO
 * TRACK THE AUTHOR of the original product to blame for or hold responsible.
 *
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Good luck and Godspeed.
 */

#define W 128
#define H 64
#define P 16
#define e ((W-(P*3))/2)
#define E (H-(2*P))

u8 C[]={128,192,224,64,0,0,0,0},B[]={0,60,126,126,126,126,60,0},l,L,r;

void on_screen(void) {
  i32 mx = mouse_x(), my = mouse_y();

  set_screen_xy(0,0);
  draw_pixel(128);
  draw_pixel(192);

  l = mx-P>e?e:mx-P<0?0:mx-P;
  L = my-P>E?E:my-P<0?0:my-P;
  r = mx-(2*P)-e>e?e:mx-(2*P)-e<0?0:mx-(2*P)-e;

  set_screen_addr(B);
  set_screen_xy(P+l-4,P+L-4);     draw_sprite(1);
  set_screen_xy(2*P+e+r-4,P+L-4); draw_sprite(1);

  set_screen_addr(C);
  set_screen_xy(mx,my);           draw_sprite(67);
}

void main(void) {
  set_palette(0xf00c, 0xf0e0, 0xf0f8);
  set_screen_size(W, H);
}
