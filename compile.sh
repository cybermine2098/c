#! /usr/bin/env bash
echo "Compiling program"
cc -o $1 $1.c $(pkg-config --cflags --libs sdl2) -lm
if [ $? -ne 0 ]; then
    echo "Compiling failed"
    exit
fi
echo "Running program."
./$1