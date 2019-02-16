#!/bin/bash

if [ ! -d "build" ]; then
    mkdir build
fi

cd build

if [[ "$OSTYPE" = "msys" ]]; then
    cmake .. -G "Visual Studio 15 2017 Win64"
    cmake --build . --config debug
    cp ../lib/sdl/windows/SDL2.dll bin/Debug/
else
    cmake ..
    cmake --build .
fi
