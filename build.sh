#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra"

clang -o dvd `pkg-config --cflags --libs glfw3 glew` $CFLAGS dvd.c
