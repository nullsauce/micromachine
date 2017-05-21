#!/bin/bash
for file in cmake-build-debug/test/pinksim/*; do
    [ -f "$file" ] && [ -x "$file" ] && "$file"
done
