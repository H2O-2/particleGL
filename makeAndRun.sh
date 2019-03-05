#!/bin/bash

cd build

if [[ "$OSTYPE" = "msys" ]]; then
    cmake --build . --config debug
    cp ../lib/sdl/windows/SDL2.dll bin/Debug/

    cd bin/Debug
    ./particleGL.exe
else
    cmake --build .

    cd bin
    ./ParticleGL
fi
