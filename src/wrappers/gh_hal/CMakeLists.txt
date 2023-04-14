# Copyright (c) 2023 Andrea Ballestrazzi

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

target_include_directories(gh_hal PUBLIC "${PROJECT_SOURCE_DIR}/src/wrappers")

# Dependencies
if(USE_CPPGPIO AND UNIX AND NOT APPLE)
    set(CPPGPIO_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src/third-party/CppGPIO/include")
    target_include_directories(gh_hal PUBLIC ${CPPGPIO_INCLUDE_DIR})

    target_link_libraries(gh_hal PRIVATE cppgpio)
    target_compile_definitions(gh_hal PRIVATE "USE_CPPGPIO")
endif()

if(USE_LIBGPIOD AND UNIX AND NOT APPLE)
    set(LIBGPIOG_NAME "libgpiod-2.0.1")
    set(LIBGPIOD_LIB_DIR ${PROJECT_SOURCE_DIR}/src/third-party/${LIBGPIOG_NAME}/lib)
    set(LIBGPIOD_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/third-party/${LIBGPIOG_NAME}/include)
    target_include_directories(gh_hal PRIVATE ${LIBGPIOD_INCLUDE_DIR})

    target_link_libraries(gh_hal PRIVATE ${LIBGPIOD_LIB_DIR}/libgpiod.a)
    target_compile_definitions(gh_hal PRIVATE "USE_LIBGPIOD")
endif()

target_link_libraries(gh_hal PUBLIC gh_log)
