# Copyright (C) 2022 Andrea Ballestrazzi

# RPI_GC's header files
set(RPI_GC_HEADER_FILES
    "rpi_gc-config-file.hpp"
)

# RPI_GC's source files
set(RPI_GC_SOURCE_FILES
    "entry-point.cpp"
)

add_executable(rpi_gc ${RPI_GC_HEADER_FILES} ${RPI_GC_SOURCE_FILES})

set_target_properties(rpi_gc
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
)

# Dependencies
target_include_directories(rpi_gc PUBLIC ${GH_LOG_INCLUDE_DIR})
target_include_directories(rpi_gc PUBLIC ${GH_HAL_INCLUDE_DIR})

target_link_libraries(rpi_gc PUBLIC gh_hal)
target_link_libraries(rpi_gc PUBLIC gh_log)
