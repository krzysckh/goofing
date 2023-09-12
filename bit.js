/* $ qjs bit.js | convert PPM:- -magnify -magnify out.png */
const print = console.log;

const width  = 2<<8;
const height = 2<<8;

const range = x      => [...Array(x).keys()];
const f     = (x, y) => (x ^ y) % 9;
const R     = (s, n) => Array(1 + n).join(s);
const V     = x      => x ? R("255 ", 3) : R("0 ", 3);

function renderPPM(c, w, h) {
  print(`P3\n${w} ${h}\n255`);

  c.forEach(x => print(V(x)));
}

function main() {
  let canvas = [];

  range(height).forEach(y => {
    range(width).forEach(x => {
      canvas.push(f(x, y));
    });
  });

  renderPPM(canvas, width, height);
}

main()
