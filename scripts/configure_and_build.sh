#!/bin/sh

# Builds swine. To be executed from a docker container where the swine
# root directory is mounted at /swine-z3/

mkdir -p /swine-z3/tmp/build
cd /swine-z3/tmp/build
cmake -DCMAKE_BUILD_TYPE=Release ../..
make -j
cp swine-z3 libswine-z3.a ../..
rm -r /swine-z3/tmp

