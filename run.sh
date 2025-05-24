#!/bin/bash

set -e

# Release build
if [ "$1" == "" ]; then
    clear
    ./reset_save_files.sh --soft
    if [ ! -d build ]; then
        mkdir build
    fi
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make
    cd ..
    ./build/bin/main
fi

# Debug build
if [ "$1" == "-d" ] || [ "$1" == "--debug" ]; then
    clear
    ./reset_save_files.sh --soft
    if [ ! -d debug ]; then
        mkdir debug
    fi
    cd debug
    cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
    make
    cd ..
    gdb -ex run ./debug/bin/main
fi

# Help info
if [ "$1" == "--help" ]; then
    echo "Usage: ./run.sh [OPTION]..."
    echo "Compile and run the program"
    echo ""
    echo "With no OPTION, compile and run the release build"
    echo ""
    echo "-d, --debug    Compile the debug build and run it with gdb"
fi
