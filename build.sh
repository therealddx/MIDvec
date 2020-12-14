#!/bin/bash

if [[ -d "$PWD"/bin ]]; then
  rm -rf "$PWD"/bin
fi

mkdir "$PWD"/bin

export PATH_SRC="$PWD"/src

gcc -I"$PATH_SRC" \
  $(find "$PATH_SRC" -type f -name '*.c') \
  -lm -o "$PWD"/bin/midvec.out

