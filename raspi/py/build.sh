#!/bin/sh

set -xe

board=/dev/ttyU0
ampy=ampy

#$ampy --port $board reset

run() {
  nam=$1
  $ampy --port $board run $nam
}

case "$1" in
  run)
    nam=$2
    run $nam
    ;;
  cp)
    echo todo
    exit 1
    ;;
  com)
    doas minicom -D $board
    ;;
esac

#[ -z "$nam" ] && exit 1


