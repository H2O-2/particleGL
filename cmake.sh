#!/bin/bash

if [ ! -d "build" ]; then
    mkdir build
fi

cd build

if [[ "$OSTYPE" = "msys" ]]; then
    cmake .. -G "Visual Studio 15 2017 Win64"
    # cmake .. -G "Unix Makefiles"
    cmake --build . --config debug
    cp ../lib/sdl/windows/SDL2.dll bin/Debug/
else
    cmake .. -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=1
    cmake --build .
    cp compile_commands.json ../compile_commands.json
fi
