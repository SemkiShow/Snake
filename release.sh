#!/bin/bash

set -e

# Compiling for Linux
if [ ! -d build ]; then
    mkdir build
fi
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-static"
make
cd ..
cp build/bin/main main

# Compiling for Windows
if [ ! -d build_win ]; then
    mkdir build_win
fi
cd build_win
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-static" --toolchain ../mingw-w64-x86_64.cmake
make
cd ..
cp build_win/bin/main.exe .

# Zipping the build
rm release.zip
./reset_save_files.sh
zip release.zip main main.exe assets/* LICENSE README.md score.txt settings.txt
rm main main.exe

# Creating a GitHub release
gh release create $1 release.zip
