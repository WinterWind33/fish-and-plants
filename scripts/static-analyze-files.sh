#! /bin/bash

for file in "$@";
do
    echo "[INFO] => Processing file $file"
    clang-tidy -p buildLinux/compile_commands.json $file -extra-arg=-std=c++20 -- \
        -DUSE_POPL20 -DUSE_LIBGPIOD -DUSE_SPDLOG \
        -Isrc/rpi_gc -Isrc/wrappers/ \
        -Isrc/third-party/popl20 -Isrc/third-party/libgpiod-2.0.1/include -Isrc/third-party/spdlog-1.11.0/include
done
