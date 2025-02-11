#!/bin/bash
cd build
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo .. &&
make &&
gdb ./build/main
