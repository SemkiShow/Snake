#!/bin/bash
# Release
cmake -B build -DCMAKE_BUILD_TYPE=Release &&
cmake --build build --config Release &&
./build/bin/main
