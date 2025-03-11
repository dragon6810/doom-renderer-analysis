#!/usr/bin/env bash

cd build
if make; then
    cd ../run
    ../build/dsda-doom -file DOOM.WAD -nomonsters

    exit 0
fi

exit 1