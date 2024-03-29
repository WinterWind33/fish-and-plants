#! /bin/bash

bShouldFail=false
vcpkgIncludeDir=$1

for file in "$@";
do
    # We only want to process .hpp and .cpp files.
    if [[ $file == *.hpp ]] || [[ $file == *.cpp ]]; then
        # We need to check whether or not the file exists. If the file doesn't
        # then we don't want to process it.
        if [ ! -f "$file" ]; then
            echo "[INFO] => Skipping the file $file because it doesn't exist."
            continue
        fi
        echo "[INFO] => Processing file $file"
        clang-tidy -p buildLinux/compile_commands.json $file -extra-arg=-std=c++20 -- \
            -DUSE_POPL20 -DUSE_LIBGPIOD -DUSE_SPDLOG \
            -Isrc/rpi_gc -Isrc/wrappers/ \
            -Isrc/third-party/popl20 -Isrc/third-party/libgpiod-2.0.1/include \
            -Ibuild/_deps/semver-src/include -Ibuild/_deps/nlohmann_json-src/include -Isrc/modules/project-management/include \
            -Isrc/modules/project-management/src -Isrc/modules/folder-provider/include -Isrc/modules/folder-provider/src \
            -Ibuild/_deps/platform_folders-src/sago -Ibuild/_deps/spdlog-src/include \
            -Ibuild/modules/workflows/src -Isrc/modules/workflows/include \
            -I$vcpkgIncludeDir
    else
        echo "[INFO] => Skipping $file because it's not C++ code."
        continue
    fi

    # If clang-tidy found some error we need to notify the caller and, if this is
    # run on a workflow/pipeline, make it fail.
    ctExitCode=$?
    echo "[INFO] => EXIT CODE: $ctExitCode"

    if [ $ctExitCode -ne 0 ]; then
        bShouldFail=true
    fi
done

if [ "$bShouldFail" = true ]; then
    echo "[ERROR] => Exiting with code 1."
    exit 1
fi
