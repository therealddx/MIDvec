#!/bin/bash

if [[ -d "$PWD"/bin ]]; then
  rm -rf "$PWD"/bin
fi

mkdir "$PWD"/bin

# know this via magic.
# 
export PATH_SRC=/home/dev/Work/midvec/src

gcc -I"$PATH_SRC" \
  main.c \
  $(find "$PATH_SRC" -type f -name '*.c' | grep -v '/app/') \
  -lm -o "$PWD"/bin/midvec.out

