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
    "commands/status-command.hpp"
    "commands/bivalent-command.hpp"
    "commands/project-command.hpp"
    "commands/automatic-watering/automatic-watering-command.hpp"
    "diagnostics/diagnostic-status-probeable.hpp"
    "gc-project/project-controller.hpp"
    "gc-project/project-component.hpp"
    "gc-project/upgraders/project-upgraders.hpp"
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
    "commands/status-command.cpp"
    "commands/project-command.cpp"
    "commands/automatic-watering/automatic-watering-command.cpp"
    "gc-project/project-controller.cpp"
    "gc-project/upgraders/project-upgraders.cpp"
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

if(USE_SEMVER)
    target_link_libraries(rpi_gc_lib PUBLIC semver)
endif()

if(GC_USE_SAGO_PLATFORM_FOLDERS)
    target_link_libraries(rpi_gc_lib PUBLIC platform_folders)
endif()

target_include_directories(rpi_gc_lib PUBLIC "${PROJECT_SOURCE_DIR}/src/rpi_gc")
target_include_directories(rpi_gc_lib PUBLIC "${PROJECT_SOURCE_DIR}/src/wrappers")
target_include_directories(rpi_gc_lib PUBLIC "${PROJECT_SOURCE_DIR}/src/modules/project-management/include")
target_include_directories(rpi_gc_lib PUBLIC "${PROJECT_SOURCE_DIR}/src/modules/workflows/include")

target_link_libraries(rpi_gc_lib PUBLIC gh_hal gh_log gh_cmd fep_workflows project_management_static Microsoft.GSL::GSL)

set(RPI_GC_EXECUTABLE_HEADER_FILES
    "initial-project-loader.hpp"
    "commands-factory.hpp"
    "project-loader.hpp"
    "application-configuration.hpp"
)

set(RPI_GC_EXECUTABLE_SOURCE_FILES
    "entry-point.cpp"
    "initial-project-loader.cpp"
    "commands-factory.cpp"
    "project-loader.cpp"
)

add_executable(rpi_gc ${RPI_GC_EXECUTABLE_SOURCE_FILES} ${RPI_GC_EXECUTABLE_HEADER_FILES})
set_target_properties(rpi_gc
    PROPERTIES
    VERSION "1.2.0"
    ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
)
set(RPI_GC_VERSION_MAJOR 1)
set(RPI_GC_VERSION_MINOR 2)
set(RPI_GC_VERSION_PATCH 0)

# Uncomment this for an RC version.
# set(RPI_GC_VERSION_RC_NUM 0)
configure_file(
    "${PROJECT_SOURCE_DIR}/src/rpi_gc/version/version-numbers.hpp.in" "${PROJECT_SOURCE_DIR}/src/rpi_gc/version/version-numbers.hpp"
    NEWLINE_STYLE LF
)

# Dependencies
# Here we don't want to include gh_hal and gh_log directly as this will result
# in possible includes clash. Instead, we include their parent directory.
target_link_libraries(rpi_gc PRIVATE rpi_gc_lib gc_folder_provider nlohmann_json Microsoft.GSL::GSL)
