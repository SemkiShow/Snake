#!/bin/bash

set -e

if [ ! -f settings.txt ] || [ ! -f score.txt ] || [ "$1" == "" ]; then
    echo "3" > score.txt
    printf "fun-mode=false\nfun-mode-level=5\nspeed=3\nmax-speed=20\nscale=50\nsnake-color=100,245,50\napple-color=255,0,0\napples-number=1\nno-speed-limit=false\nauto-mode=false\nvsync=true\naudio-volume=0.5f\n" > settings.txt
fi
