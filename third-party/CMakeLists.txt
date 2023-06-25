
if(USE_SEMVER)
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/third-party/semver/include/semver.hpp")
        message(STATUS "Downloading semver.hpp from Neargye/semver Github repo.")

        file(DOWNLOAD
            "https://raw.githubusercontent.com/Neargye/semver/master/include/semver.hpp"
            "${PROJECT_SOURCE_DIR}/third-party/semver/include/semver.hpp"
        )
    else()
        message(STATUS "Skipping download of semver.hpp: file already exists.")
    endif()
endif()
