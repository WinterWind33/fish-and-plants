# Copyright (C) 2022 Andrea Ballestrazzi

# RPI_GC's header files
set(RPI_GC_HEADER_FILES
    "rpi_gc-config-file.hpp"
    "greenhouse-controller-application.hpp"
    "application/application.hpp"
    "common/types.hpp"
    "commands/command.hpp"
    "commands/terminal-command.hpp"
    "commands/application-command.hpp"
)

# RPI_GC's source files
set(RPI_GC_SOURCE_FILES
    "greenhouse-controller-application.cpp"
    "commands/application-command.cpp"
)

# Here we add a library target so we can use it to link it against
# the test app.
add_library(rpi_gc_lib STATIC ${RPI_GC_HEADER_FILES} ${RPI_GC_SOURCE_FILES})
set_target_properties(rpi_gc_lib
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
)

target_include_directories(rpi_gc_lib PUBLIC "${PROJECT_SOURCE_DIR}/src/rpi_gc")
target_include_directories(rpi_gc_lib PUBLIC "${PROJECT_SOURCE_DIR}/src/wrappers")

target_link_libraries(rpi_gc_lib PRIVATE gh_hal)
target_link_libraries(rpi_gc_lib PRIVATE gh_log)
target_link_libraries(rpi_gc_lib PUBLIC gh_cmd)

add_executable(rpi_gc "entry-point.cpp")

set_target_properties(rpi_gc
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
)

# Dependencies
# Here we don't want to include gh_hal and gh_log directly as this will result
# in possible includes clash. Instead, we include their parent directory.
target_include_directories(rpi_gc PUBLIC "${PROJECT_SOURCE_DIR}/src/rpi_gc")
target_include_directories(rpi_gc PUBLIC "${PROJECT_SOURCE_DIR}/src/wrappers")

target_link_libraries(rpi_gc PRIVATE rpi_gc_lib)
