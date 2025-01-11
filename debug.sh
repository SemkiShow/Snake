#!/bin/bash
cd build
g++ -c -g ../src/*.cpp &&
g++ *.o -g -o main -lsfml-graphics -lsfml-window -lsfml-system &&
cd .. &&
gdb ./build/main
