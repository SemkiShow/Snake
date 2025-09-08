#!/bin/bash

set -e

# Compiling for Linux
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j ${nproc}
cp build/bin/main main

# Compiling for Windows
cmake -B build_win -DCMAKE_BUILD_TYPE=Release --toolchain ../mingw-w64-x86_64.cmake
cmake --build build_win -j ${nproc}
cp build_win/bin/main.exe .

# Zipping the build
./reset_save_files.sh
zip release.zip main main.exe resources/* LICENSE README.md score.txt settings.txt
rm main main.exe

# Creating a GitHub release
gh release create $1 release.zip
rm release.zip
