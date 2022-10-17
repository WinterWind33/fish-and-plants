# Copyright (c) 2022 Andrea Ballestrazzi

set(GH_HAL_SOURCE_FILES
    "hal-context.cpp"
    "hal-board-pin.cpp"
)
set(GH_HAL_HEADER_FILES
    "hal-context.hpp"
    "hal-error.hpp"
    "hal-definitions.hpp"
    "hal-digital-output.hpp"
    "hal-board-pin.hpp"
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

set(GH_HAL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src/wrappers/gh_hal")

# Dependencies
target_include_directories(gh_hal PUBLIC ${GH_HAL_INCLUDE_DIR})
target_include_directories(gh_hal PUBLIC "${PROJECT_SOURCE_DIR}/src/wrappers")

target_link_libraries(gh_hal PUBLIC gh_log)

if(USE_CPPGPIO AND UNIX)
    set(CPPGPIO_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src/third-party/CppGPIO/include/")
    message("[INFO] => Linking CppGPIO to gh_hal. Include dir: ${CPPGPIO_INCLUDE_DIR}")

    target_include_directories(gh_hal PUBLIC "${PROJECT_SOURCE_DIR}/src/third-party/CppGPIO/include/")
    target_link_libraries(gh_hal PUBLIC cppgpio)
    target_compile_definitions(gh_hal PUBLIC "USE_CPPGPIO")
endif()
