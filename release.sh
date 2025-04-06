#!/bin/bash
# Compiling for Linux
mkdir build
cd build &&
cmake .. -DCMAKE_BUILD_TYPE=Release &&
make &&
cd ..
cp build/bin/main main

# Compiling for Windows
mkdir build_win
cd build_win &&
cmake .. -DCMAKE_BUILD_TYPE=Release --toolchain ../mingw-w64-x86_64.cmake &&
make &&
cd ..
cp build_win/bin/main.exe .

# Zipping the build
rm release.zip
echo $1 > version.txt
echo "3" > score.txt
printf "fun-mode=false\nfun-mode-level=5\nspeed=3\nmax-speed=20\nscale=50\nsnake-color=0.392157,0.980392,0.196078\napple-color=1,0,0\napples-number=1\nno-speed-limit=false\nauto-mode=false\n" > settings.txt
zip release.zip main main.exe libgcc_s_seh-1.dll libstdc++-6.dll assets/* LICENSE README.md score.txt settings.txt version.txt
rm main main.exe

# Making a new GitHub commit
git add .
git commit
git push origin main

# Creating a GitHub release
gh release create $1 release.zip
