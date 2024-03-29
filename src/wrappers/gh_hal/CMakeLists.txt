# Copyright (c) 2023 Andrea Ballestrazzi

set(GH_HAL_SOURCE_FILES
    # Backends implementations
    "backends/simulated/simulated-chip.cpp"

    # Internal implementations
    "internal/board-chip-impl.cpp"
    "internal/board-digital-pin-impl.cpp"
    "internal/line-request.cpp"

    # Hardware access and main API
    "hardware-access/board-chip.cpp"
    "hardware-abstraction-layer.cpp"
)

set(GH_HAL_HEADER_FILES
    "backends/simulated/simulated-chip.hpp"
    "backends/simulated/simulated-digital-board-pin.hpp"
    "hardware-access/board-chip.hpp"
    "hardware-access/board-digital-pin.hpp"
    "hardware-abstraction-layer.hpp"

    "internal/board-chip-impl.hpp"
    "internal/board-digital-pin-impl.hpp"
    "internal/line-request.hpp"
)

if(USE_LIBGPIOD AND UNIX AND NOT APPLE)

    list(APPEND GH_HAL_SOURCE_FILES
        "backends/libgpiod/miscellaneous.cpp"
        "backends/libgpiod/chip-api.cpp"
    )

    list(APPEND GH_HAL_HEADER_FILES
        "backends/libgpiod/miscellaneous.hpp"
        "backends/libgpiod/chip-api.hpp"
    )

endif()

# We create a static library
add_library(gh_hal STATIC ${GH_HAL_HEADER_FILES} ${GH_HAL_SOURCE_FILES})

# Compilation configurations.
set_target_properties(gh_hal
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
)

target_include_directories(gh_hal PRIVATE "${PROJECT_SOURCE_DIR}/src/wrappers")

# Dependencies

if(USE_LIBGPIOD AND UNIX AND NOT APPLE)
    set(LIBGPIOG_NAME "libgpiod-${LIBGPIOD_VERSION}")
    set(LIBGPIOD_LIB_DIR ${PROJECT_SOURCE_DIR}/src/third-party/${LIBGPIOG_NAME}/lib)
    set(LIBGPIOD_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/third-party/${LIBGPIOG_NAME}/include)
    target_include_directories(gh_hal PRIVATE ${LIBGPIOD_INCLUDE_DIR})

    # We link the C++ bindings.
    target_link_libraries(gh_hal PRIVATE ${LIBGPIOD_LIB_DIR}/libgpiodcxx.a)

    # If we lin the C++ binding we also need to link the origina library because
    # libgpiod low-level symbols are defined there.
    target_link_libraries(gh_hal PRIVATE ${LIBGPIOD_LIB_DIR}/libgpiod.a)
    target_compile_definitions(gh_hal PRIVATE "USE_LIBGPIOD")
endif()

target_link_libraries(gh_hal PRIVATE gh_log)
