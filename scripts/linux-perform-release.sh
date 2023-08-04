#! /bin/bash

# aarch64 cross compile on linux
echo "[INFO] => Compiling for Linux aarch64"
cd ../buildLinux
rm -r -f ./*
rm -r -f ../src/third-party/libgpiod-*
rm ../bin/rpi_gc*
cmake .. -DCOMPILE_AARCH64_ON_LINUX=True --toolchain "toolchains/aarch64-toolchain.cmake"
cmake --build . --target rpi_gc --config Release -j8
echo "[INFO] => Copying to bin/release/aarch64-linux"
mkdir -p ../bin/Release/aarch64-linux
cp ../bin/rpi_gc* ../bin/Release/aarch64-linux

# Ubuntu x64 release
echo "[INFO] => Compiling for Linux x64"
rm -r -f ./*
rm -r -f ../src/third-party/libgpiod-*
rm ../bin/rpi_gc*
cmake .. --toolchain "toolchains/linux-toolchain.cmake"
cmake --build . --target rpi_gc --config Release -j8
echo "[INFO] => Copying to bin/release/linux-x64"
mkdir -p ../bin/Release/linux-x64
cp ../bin/rpi_gc* ../bin/Release/linux-x64
