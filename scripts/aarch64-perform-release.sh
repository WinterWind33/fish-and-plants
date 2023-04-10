#! /bin/bash

cd ../buildLinux
echo "[INFO] => Setting the working dir to: "
pwd
rm -r -f ./*
cmake .. -DCOMPILE_AARCH64_ON_LINUX=True --toolchain "toolchains/aarch64-toolchain.cmake" -Wno-dev
cmake --build . --config Release
