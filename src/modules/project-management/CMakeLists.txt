set(PRJ_MGMT_INCLUDE_ABS_DIR "${PROJECT_SOURCE_DIR}/src/modules/project-management/include")
set(PRJ_MGMT_SRC_DIR "${PROJECT_SOURCE_DIR}/src/modules/project-management/src")

set(PRJ_MGMT_INCLUDE_FILES
    "include/project-management/project.hpp"
    "include/project-management/integrity-check/project-integrity-checker.hpp"
    "include/project-management/integrity-check/title-integrity-checker.hpp"
    "include/project-management/integrity-check/version-integrity-checker.hpp"
    "include/project-management/project-io/project-writer.hpp"
    "include/project-management/project-io/project-reader.hpp"

    # Private includes.
    "src/project-io/json-project-writer.hpp"
    "src/project-io/json-project-reader.hpp"
)

set(PRJ_MGMT_SOURCE_FILES
    "src/integrity-check/version-integrity-checker.cpp"
    "src/integrity-check/title-integrity-checker.cpp"
    "src/project-io/project-writer.cpp"
    "src/project-io/project-reader.cpp"
    "src/project-io/json-project-writer.cpp"
    "src/project-io/json-project-reader.cpp"
)

add_library(project_management_static STATIC ${PRJ_MGMT_INCLUDE_FILES} ${PRJ_MGMT_SOURCE_FILES})
set_target_properties(project_management_static
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    LINKER_LANGUAGE CXX
)

target_include_directories(project_management_static PUBLIC ${PRJ_MGMT_INCLUDE_ABS_DIR})
target_include_directories(project_management_static PRIVATE ${PRJ_MGMT_SRC_DIR})

if(USE_SEMVER)
    target_link_libraries(project_management_static PUBLIC semver)
endif()

if(GC_USE_NLOHMANN_JSON)
    target_link_libraries(project_management_static PRIVATE nlohmann_json::nlohmann_json)
endif()
