#!/bin/bash

set -e

if [ "$1" == "" ]; then
    rm settings.txt score.txt
fi
