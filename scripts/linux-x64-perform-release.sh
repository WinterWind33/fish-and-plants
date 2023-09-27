# Ubuntu x64 release
echo "[INFO] => Compiling for Linux x64"
rm -r -f ./*
rm -r -f ../src/third-party/libgpiod-*
rm ../bin/rpi_gc*
cmake .. --toolchain "toolchains/linux-toolchain.cmake" -DRPI_GC_BUILD_TESTS=OFF
cmake --build . --target rpi_gc --config Release -j8
echo "[INFO] => Copying to bin/release/linux-x64"
mkdir -p ../bin/Release/linux-x64
cp ../bin/rpi_gc* ../bin/Release/linux-x64
