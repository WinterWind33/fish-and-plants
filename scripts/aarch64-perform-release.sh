#! /bin/bash

cd ../buildLinux
#rm -r -f ./*
cmake .. -DCOMPILE_AARCH64_ON_LINUX=True --toolchain "toolchains/aarch64-toolchain.cmake" -Wno-dev
cmake --build . --config Release
