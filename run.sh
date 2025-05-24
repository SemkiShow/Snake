#!/bin/bash

set -e

if [ "$1" == "" ]; then
    clear
    ./compile.sh
    ./build/bin/main
fi
if [ "$1" == "-d" ] || [ "$1" == "--debug" ]; then
    clear
    ./debug.sh
    gdb -ex run ./debug/bin/main
fi
