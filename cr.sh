#! /usr/bin/env bash
if [ -f "$1" ] && [ "$1" -nt "$1.c" ]; then
    echo -e "\e[33mSkipping compilation (executable is up to date)\e[0m"
else
    echo -e "\e[32mCompiling program\e[0m"
    cc -o $1 $1.c $(pkg-config --cflags --libs sdl2 ) -lSDL2_mixer -lm
    if [ $? -ne 0 ]; then
        echo -e "\e[31mCompiling failed\e[0m"
        exit
    fi
    echo -e "\e[32mCompiled successfully\e[0m"
fi

if [ "$2" != "-r" ]; then
    exit
fi
echo -e "\e[36mRunning program.\e[0m"
./$1
