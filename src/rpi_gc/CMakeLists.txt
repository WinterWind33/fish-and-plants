# Copyright (C) 2023 Andrea Ballestrazzi

# RPI_GC's header files
set(RPI_GC_HEADER_FILES
    "version/version-numbers.hpp"
    "greenhouse-controller-application.hpp"
    "abort-system/emergency-stoppable-system.hpp"
    "abort-system/terminable-system.hpp"
    "application/application.hpp"
    "automatic-watering/automatic-watering-system.hpp"
    "automatic-watering/daily-cycle-automatic-watering-system.hpp"
    "automatic-watering/hardware-controllers/watering-system-hardware-controller.hpp"
    "automatic-watering/hardware-controllers/daily-cycle-aws-hardware-controller.hpp"
    "automatic-watering/time-providers/watering-system-time-provider.hpp"
    "automatic-watering/time-providers/daily-cycle-aws-time-provider.hpp"
    "automatic-watering/time-providers/configurable-daily-cycle-aws-time-provider.hpp"
    "common/types.hpp"
    "commands/command.hpp"
    "commands/abort-command.hpp"
    "commands/terminal-command.hpp"
    "commands/application-command.hpp"
    "commands/version-command.hpp"
    "commands/help-command.hpp"
    "commands/bivalent-command.hpp"
    "commands/automatic-watering/automatic-watering-command.hpp"
    "hardware-management/hardware-chip-initializer.hpp"
    "user-interface/application-strings.hpp"
    "user-interface/commands-strings.hpp"
)

# RPI_GC's source files
set(RPI_GC_SOURCE_FILES
    "greenhouse-controller-application.cpp"
    "commands/application-command.cpp"
    "commands/version-command.cpp"
    "commands/help-command.cpp"
    "commands/abort-command.cpp"
    "commands/automatic-watering/automatic-watering-command.cpp"
    "automatic-watering/daily-cycle-automatic-watering-system.cpp"
    "automatic-watering/hardware-controllers/daily-cycle-aws-hardware-controller.cpp"
    "automatic-watering/time-providers/configurable-daily-cycle-aws-time-provider.cpp"
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
target_link_libraries(rpi_gc_lib PRIVATE gh_cmd)

add_executable(rpi_gc "entry-point.cpp")
set_target_properties(rpi_gc
    PROPERTIES
    VERSION "1.0.0"
    ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
)
set(RPI_GC_VERSION_MAJOR 1)
set(RPI_GC_VERSION_MINOR 0)
set(RPI_GC_VERSION_PATCH 0)
set(RPI_GC_VERSION_RC_NAME "rc-1")
configure_file("${PROJECT_SOURCE_DIR}/src/rpi_gc/rpi_gc-config-file.hpp.in" "${PROJECT_SOURCE_DIR}/src/rpi_gc/version/version-numbers.hpp")

# Dependencies
# Here we don't want to include gh_hal and gh_log directly as this will result
# in possible includes clash. Instead, we include their parent directory.
target_include_directories(rpi_gc PRIVATE "${PROJECT_SOURCE_DIR}/src/rpi_gc")
target_include_directories(rpi_gc PRIVATE "${PROJECT_SOURCE_DIR}/src/wrappers")

target_link_libraries(rpi_gc PRIVATE gh_log)
target_link_libraries(rpi_gc PRIVATE gh_cmd)
target_link_libraries(rpi_gc PRIVATE rpi_gc_lib)
