#! /bin/bash

cd ../buildLinux
rm -r -f ./*
cmake .. -Wno-dev --toolchain "toolchains/clang-linux-toolchain.cmake" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
