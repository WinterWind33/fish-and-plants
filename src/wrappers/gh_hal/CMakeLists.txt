# Copyright (c) 2022 Andrea Ballestrazzi

set(GH_HAL_SOURCE_FILES
    "hal-context.cpp"
)
set(GH_HAL_HEADER_FILES
    "hal-context.hpp"
)

# We create a static library
add_library(gh_hal STATIC ${GH_HAL_HEADER_FILES} ${GH_HAL_SOURCE_FILES})

# Compilation configurations.
set_target_properties(gh_hal
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
)

set(GH_HAL_INCLUDE_DIR ".")

# Dependencies
target_include_directories(gh_hal PUBLIC ${GH_HAL_INCLUDE_DIR})
target_include_directories(gh_log PUBLIC ${GH_LOG_INCLUDE_DIR})

target_link_libraries(gh_hal PUBLIC gh_log)