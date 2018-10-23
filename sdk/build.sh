#!/usr/bin/env bash

mkdir -p build
pushd build
cmake \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DCMAKE_TOOLCHAIN_FILE=../micromachine.toolchain \
    ..
make