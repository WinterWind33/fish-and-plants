#! /bin/bash

cd buildLinux
#rm -r -f ./*
cmake .. -DCOMPILE_AARCH64_ON_LINUX=True -DCOMPILE_FOR_CORTEX_A53=True --toolchain "toolchains/aarch64-toolchain.cmake"
cmake --build . --config Release
