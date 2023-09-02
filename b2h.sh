#!/bin/sh
#
# requires pc (https://git.sr.ht/~ft/pc)

while read i; do
  echo "hex($(echo $i | tr -d ','))" | pc | tr '\n' ','
done
