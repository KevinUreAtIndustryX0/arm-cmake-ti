#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $DIR
clean=false
while [[ "$#" -gt 0 ]]
do
    case $1 in
        -c|--clean)
            clean=true
            ;;
    esac
    shift
done
touch ./CMakeLists.txt
mkdir -p build
if [ "$clean" = true ]
then
    rm -rf ./build/*
fi
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../target-toolchain.cmake ..
if [ $? -ne 0 ]
then
  echo "cmake failed"
  exit 1
fi
make
