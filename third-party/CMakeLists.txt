
if(USE_SEMVER)
    set(SEMVER_HPP_FILE_ABS_PATH "${PROJECT_SOURCE_DIR}/third-party/semver/include/semver.hpp")

    if(NOT EXISTS ${SEMVER_HPP_FILE_ABS_PATH})
        message(STATUS "Downloading semver.hpp from Neargye/semver Github repo.")

        file(DOWNLOAD
            "https://raw.githubusercontent.com/Neargye/semver/master/include/semver.hpp"
            ${SEMVER_HPP_FILE_ABS_PATH}
        )
    else()
        message(STATUS "Skipping download of semver.hpp: file already exists.")
    endif()

    add_library(semver_header_only INTERFACE)
    target_include_directories(semver_header_only INTERFACE "${PROJECT_SOURCE_DIR}/third-party/semver/include")
    set_target_properties(semver_header_only
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
        LINKER_LANGUAGE CXX
    )
endif()
