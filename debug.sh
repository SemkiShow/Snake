#!/bin/bash
mkdir build
cd build &&
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo .. &&
make &&
cd .. &&
gdb ./build/bin/main
