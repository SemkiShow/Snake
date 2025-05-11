#!/bin/bash

set -e

./reset_save_files.sh --soft
if [ ! -d build ]; then
    mkdir build
fi
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
cd ..
