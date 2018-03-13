#!/usr/bin/env bash

mkdir -p build
pushd build
cmake -DCMAKE_TOOLCHAIN_FILE=../micromachine.toolchain ..
make