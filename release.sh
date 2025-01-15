#!/bin/bash
# Compiling for Linux
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cp build/bin/main main

# Compiling for Windows
# cd build_win
# cmake .. --toolchain ../mingw-w64-x86_64.cmake
# make
# cd ..
# cp build_win/bin/main.exe main.exe

# Zipping the build
rm release.zip
# zip release.zip main main.exe JetBrainsMonoNerdFont-Medium.ttf LICENSE README.md
# rm main main.exe
zip release.zip main JetBrainsMonoNerdFont-Medium.ttf LICENSE README.md
rm main

# Creating a GitHub release
gh release create
