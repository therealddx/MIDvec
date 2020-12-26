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
  $(find "$PATH_SRC"/dsp/ -type f -name '*.c') \
  $(find "$PATH_SRC"/math/ -type f -name '*.c') \
  -lm -o "$PWD"/bin/midvec.out

