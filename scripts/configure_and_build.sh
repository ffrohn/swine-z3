#!/bin/bash

# Reconfigures the cmake build directory and triggers a build. To be executed
# from a docker container where the LoAT root directory is mounted at
# /LoAT/. Reconfiguration is necessary to get the information whether the
# working tree is clean or not.

mkdir -p /swine-z3/build
cd /swine-z3/build
cmake -DCMAKE_BUILD_TYPE=Release ../
make -j
