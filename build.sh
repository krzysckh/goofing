#!/bin/sh

set -xe

CC=clang
CFLAGS="-Wall -Wextra -I/usr/local/include"

build_uxn() {
  [ ! -d ./chibicc ] && git submodule update --init --recursive
  [ ! -f ./chibicc/chibicc ] && {
    cd chibicc || exit 1
    make
    cd ..
  }

  in="$1"
  out="$2"

  $CC -Ichibicc -P -E "$in" -o "$in".cpp.c
  ./chibicc/chibicc -O1 "$in.cpp.c" > "$in.tal"
  uxnasm "$in.tal" "$out"

  rm -f "$in.cpp.c" "$in.tal" "$out.sym"
}

build() {
  $CC -o dvd `pkg-config --cflags --libs glfw3 glew` -lm $CFLAGS dvd.c
  build_uxn "./sand-uxn.c" "./sand-uxn.rom"
  build_uxn "./snake.c" "./snake.rom"
  build_uxn "./saper.c" "./saper.rom"
  build_uxn "./sheet.c" "./sheet.rom"
  build_uxn "./uxneyes.c" "./uxneyes.rom"
  build_uxn "./line-test.c" "./line-test.rom"
  build_uxn "./bit-demo.c" "./bit-demo.rom"
}

clean() {
  rm -f ./*.rom dvd
}

publish() {
  build

  for i in *.rom; do
    yes | pubcpy "$i"
  done
}

case "$1" in
  clean)
    clean
    ;;
  publish)
    publish
    ;;
  *)
    build
    ;;
esac
