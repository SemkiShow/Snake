#!/bin/bash

set -e
executable_name=Snake

# Release build
if [ "$1" == "" ]; then
    clear
    cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
    cmake --build build -j32
    ./build/bin/$executable_name
fi

# Debug build
if [ "$1" == "-d" ] || [ "$1" == "--debug" ]; then
    clear
    cmake -B build_debug -DCMAKE_BUILD_TYPE=Debug
    cmake --build build_debug -j32
    gdb -ex run --args ./build_debug/bin/$executable_name
fi

# Windows build
if [ "$1" == "-w" ] || [ "$1" == "--windows" ]; then
    clear
    cmake -B build_windows -DCMAKE_TOOLCHAIN_FILE="$(pwd)/mingw-w64-x86_64.cmake" -DCMAKE_BUILD_TYPE=RelWithDebInfo
    cmake --build build_windows -j32
    wine ./build_windows/bin/$executable_name.exe
fi

# Web build
if [ "$1" == "--web" ]; then
    clear
    if [ "$2" == "-m" ] || [ "$2" == "--minimal" ]; then
        emcmake cmake -B build_web -DPLATFORM=Web -DSHELL=Minimal
    else
        emcmake cmake -B build_web -DPLATFORM=Web -DSHELL=Full
    fi
    cmake --build build_web -j ${nproc}
    emrun ./build_web/bin/
fi

# Help info
if [ "$1" == "--help" ]; then
    echo "Usage: ./run.sh [OPTION]..."
    echo "Compile and run the program"
    echo ""
    echo "With no OPTION, compile and run the release build"
    echo ""
    echo "-d, --debug      Compile the debug build and run it with gdb"
    echo "-w, --windows    Compile the Windows build and run it with Wine"
    echo "--web      Compile the WebAssembly build"
    echo "-m, --minimal  Compile a WebAssembly build with the minimal shell"
fi
