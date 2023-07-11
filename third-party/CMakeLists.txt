
include(FetchContent)

if(USE_SEMVER)
    message(STATUS "Using semver.")

    FetchContent_Declare(semver
        GIT_REPOSITORY "https://github.com/Neargye/semver.git"
        GIT_TAG origin/master
    )

    FetchContent_MakeAvailable(semver)
endif()

if(GC_USE_NLOHMANN_JSON)
    message(STATUS "Using NLohmann/json as json engine")

    FetchContent_Declare(nlohmann_json
        GIT_REPOSITORY "https://github.com/nlohmann/json.git"
        GIT_TAG v3.11.2
    )

    FetchContent_MakeAvailable(nlohmann_json)
endif()
