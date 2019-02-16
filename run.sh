#!/bin/bash

if [[ "$OSTYPE" = "msys" ]]; then
    cd build/bin/Debug
    ./particleGL.exe
else
    cd build/bin
    ./particleGL
fi
