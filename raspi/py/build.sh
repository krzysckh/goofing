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
  install-py)
    doas mount /dev/sd1i /mnt
    cd /mnt
    doas wget https://micropython.org/download/rp2-pico/rp2-pico-latest.uf2
    ;;
esac

#[ -z "$nam" ] && exit 1


