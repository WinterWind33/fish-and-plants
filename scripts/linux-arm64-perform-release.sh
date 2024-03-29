#! /bin/bash

# arm64 cross compile on linux
echo "[INFO] => Compiling for Linux aarch64"
cd ../buildLinux
rm -r -f ./*
rm -r -f ../src/third-party/libgpiod-*
rm ../bin/rpi_gc*
cmake .. -DCOMPILE_AARCH64_ON_LINUX=True --toolchain "toolchains/aarch64-toolchain.cmake" -DRPI_GC_BUILD_TESTS=OFF
cmake --build . --target rpi_gc --config Release -j8
echo "[INFO] => Copying to bin/release/aarch64-linux"
mkdir -p ../bin/Release/aarch64-linux
cp ../bin/rpi_gc* ../bin/Release/aarch64-linux
