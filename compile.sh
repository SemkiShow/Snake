#!/bin/bash
cd build &&
cmake .. -DCMAKE_BUILD_TYPE=Release &&
make &&
cd .. &&
./build/bin/main
