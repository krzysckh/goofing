#!/bin/sh

set -xe

CC=clang
CFLAGS="-Wall -Wextra"

build_uxn() {
  [ ! -f ./chibicc ] && git submodule update --init --recursive
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

  rm -f "$in.cpp.c" "$in.tal"
}

$CC -o dvd `pkg-config --cflags --libs glfw3 glew` -lm $CFLAGS dvd.c
build_uxn "./sand-uxn.c" "./sand-uxn.rom"
