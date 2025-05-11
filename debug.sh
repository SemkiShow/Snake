#!/bin/bash

set -e

./reset_save_files.sh --soft
if [ ! -d debug ]; then
    mkdir debug
fi
cd debug
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
make
cd ..
