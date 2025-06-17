#!/bin/bash

# Compiler options
CC=g++
CFLAGS="-O1 -Wall -std=c++17 -Wno-missing-braces"

# Paths
INCLUDE_DIR="include/"
LIB_DIR="mlib"

# Build command
$CC main.cpp -o app $CFLAGS -I $INCLUDE_DIR $LIB_DIR/libraylib.a -L $LIB_DIR -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
./app