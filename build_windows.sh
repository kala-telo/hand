#!/bin/sh
# this file mostly exists for me to not forget the exact command line
# not really intended for anyone else to use
# so change SDL path yourself
# SDL is configured with
# `cmake ../CMakeLists.txt -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ -DSDL_STATIC=ON -DSDL_SHARED=OFF -DCMAKE_C_FLAGS='-flto -O3'`
x86_64-w64-mingw32-gcc hand.c -O3 -L $HOME/Programs/SDL/build/ -lSDL3 -static -I $HOME/Programs/SDL/include/ -flto=auto -lmingw32 -mwindows -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc -lsetupapi -lhid -Wl,--as-needed -o hand.exe
