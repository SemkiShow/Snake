#!/bin/bash

set -e
executable_name=Pseudo3DRenderer

# Compiling for Linux
cmake -B build_release -DCMAKE_BUILD_TYPE=Release
cmake --build build_release -j
cp build_release/bin/$executable_name .

# Compiling for Windows
cmake -B build_release_windows -DCMAKE_TOOLCHAIN_FILE=mingw-w64-x86_64.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build_release_windows -j
cp build_release_windows/bin/$executable_name.exe .

# Zipping the build
zip release.zip $executable_name $executable_name.exe LICENSE README.md resources/**
rm $executable_name $executable_name.exe

# Creating a GitHub release
gh release create $1 release.zip
rm release.zip
