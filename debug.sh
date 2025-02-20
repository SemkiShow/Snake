#!/bin/bash
mkdir debug
cd debug &&
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo .. &&
make &&
cd .. &&
gdb ./debug/bin/main
