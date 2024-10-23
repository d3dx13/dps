#!/bin/bash

mkdir -p ./include/dps/msg/
rm -rf ./include/dps/msg/*
flatc --gen-mutable --gen-object-api --cpp -o ./include/dps/msg/ ./msg/*.fbs # --python

# rm -rf ./build && \
mkdir -p build && \
cd build && \
cmake .. && \
make -j $(nproc) && \
cd -
