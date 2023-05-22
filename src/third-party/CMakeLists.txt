# Copyright (c) 2023 Andrea Ballestrazzi

include(FetchContent)

if(USE_SPDLOG)
    message(STATUS "Using spdlog v${SPDLOG_VERSION} as logger framework.")

    FetchContent_Declare(spdlog
        URL "https://github.com/gabime/spdlog/archive/refs/tags/v${SPDLOG_VERSION}.zip"
        SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/third-party/spdlog-${SPDLOG_VERSION}"
    )

    FetchContent_MakeAvailable(spdlog)

    set_target_properties(spdlog
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    )
endif()

if(USE_POPL20)
    message(STATUS "Using popl20 header only library as command-line interpreter.")

    add_subdirectory("popl20")
endif()

if(USE_LIBGPIOD AND UNIX AND NOT APPLE)
    message(STATUS "Using libgpiod as as the main I/O communication module.")

    set(LIBGPIOD_NAME "libgpiod-${LIBGPIOD_VERSION}")

    set(LIBGPIOD_FILENAME "${LIBGPIOD_NAME}.tar.gz")
    set(LIBGPIOD_PARENT_FOLDER_PATH "${PROJECT_SOURCE_DIR}/src/third-party/_temp")
    set(LIBGPIOD_FILEPATH "${LIBGPIOD_PARENT_FOLDER_PATH}/${LIBGPIOD_FILENAME}")

    # We need to download the libgpiod v2.0 tar.gz file and extract it inside the
    # third-party folder
    file(DOWNLOAD
        "https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/snapshot/${LIBGPIOD_FILENAME}"
        ${LIBGPIOD_FILEPATH})

    # Now we need to extract the file.
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${LIBGPIOD_FILENAME}
        WORKING_DIRECTORY ${LIBGPIOD_PARENT_FOLDER_PATH}
    )

    # Cleanup: we don't need the tar.gz file anymore
    #file(REMOVE ${LIBGPIOD_FILEPATH})

    set(LIBGPIOD_ABS_FOLDER ${LIBGPIOD_PARENT_FOLDER_PATH}/${LIBGPIOD_NAME})

    # in order to make it properly work the user must run the following command:
    # apt install pkg-config m4 libtool automake autoconf
    # Here we need to add a custom target that can build the library.
    set(LIBGPIOD_INSTALL_DIR ${PROJECT_SOURCE_DIR}/src/third-party/${LIBGPIOD_NAME})
    file(MAKE_DIRECTORY ${LIBGPIOD_INSTALL_DIR})

    # Configure the library's build system
    message(STATUS "Configuring libgpiod build system.")
    if(COMPILE_AARCH64_ON_LINUX)
        message(STATUS "Using aarch64-linux-gnu host for libgpiod.")

        # In order to generate the correct makefile (with CC=aarch64-linux-gnu-gcc and family) we
        # need to pass the --host=aarch64-linux-gnu (compiler triplet) to the "./configure script" run
        # by autotools.
        # We also want to enable C++ bindings as we are developing in C++ and they support smart pointers
        # for the resources.
        execute_process(
            COMMAND ./autogen.sh --enable-tools=no --enable-bindings-cxx=yes --prefix=${LIBGPIOD_INSTALL_DIR} --host=aarch64-linux-gnu
            WORKING_DIRECTORY ${LIBGPIOD_ABS_FOLDER}
        )
    else()
        # We also want to enable C++ bindings as we are developing in C++ and they support smart pointers
        # for the resources.
        execute_process(
            COMMAND ./autogen.sh --enable-tools=no --enable-bindings-cxx=yes --prefix=${LIBGPIOD_INSTALL_DIR}
            WORKING_DIRECTORY ${LIBGPIOD_ABS_FOLDER}
        )
    endif()

    # Make the library
    message(STATUS "Compiling libgpiod.")
    execute_process(
        COMMAND make
        WORKING_DIRECTORY ${LIBGPIOD_ABS_FOLDER}
    )

    # Install the target.
    message(STATUS "Installing libgpdio to ${LIBGPIOD_INSTALL_DIR}.")
    execute_process(
        COMMAND make install
        WORKING_DIRECTORY ${LIBGPIOD_ABS_FOLDER}
    )

    # Now we can entirely remove the _temp folder.
    file(REMOVE_RECURSE ${LIBGPIOD_PARENT_FOLDER_PATH})
endif()
