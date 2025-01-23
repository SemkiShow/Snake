#!/bin/bash
# Compiling for Linux
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cp build/bin/main main

# Compiling for Windows
cd build_win
cmake .. --toolchain ../mingw-w64-x86_64.cmake
make
cd ..
cp build_win/bin/main.exe .

# Zipping the build
rm release.zip
echo "3" > score.txt
printf "fun-mode=false\nfun-mode-level=5" > settings.txt
zip release.zip main main.exe libgcc_s_seh-1.dll libstdc++-6.dll assets/* LICENSE README.md score.txt settings.txt
rm main main.exe

# Making a new GitHub commit
git add .
git commit
git push origin main

# Creating a GitHub release
gh release create $1 release.zip
