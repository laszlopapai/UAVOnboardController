#!/bin/bash

. ./functions.sh

PWD_=$PWD
PROJECT=opencv
CORES=${CORES:-`grep -c ^processor /proc/cpuinfo`}

BUILD_PATH=$PWD_/.CMakeBuild/$PROJECT
FILES_PATH=$PWD_/.CMakeFiles/$PROJECT

rm -rf $BUILD_PATH
rm -rf $FILES_PATH

echo "BUILDING $PROJECT"
mkdir -p $BUILD_PATH
mkdir -p $FILES_PATH
cd $FILES_PATH
run cmake ../../$PROJECT -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=$BUILD_PATH -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF
run cmake --build . -- -j$CORES | tee $PWD_/$PROJECT.log

run cmake --build . --target install -- -j$CORES

cd $PWD_