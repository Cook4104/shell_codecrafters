#!/bin/sh
#
# This script is used to compile your program on CodeCrafters
# 
# This runs before .codecrafters/run.sh
#
# Learn more: https://codecrafters.io/program-interface

# Exit early if any commands fail
set -e

echo Compiling Program!

./my_exe David

cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
cmake --build ./build

chmod u+x ./.codecrafters/run.sh 
