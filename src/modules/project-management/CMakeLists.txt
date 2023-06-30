set(PRJ_MGMT_INCLUDE_ABS_DIR "${PROJECT_SOURCE_DIR}/src/modules/project-management/include")

set(PRJ_MGMT_INCLUDE_FILES
    "include/project-management/project.hpp"
    "include/project-management/integrity-check/project-integrity-checker.hpp"
    "include/project-management/integrity-check/title-integrity-checker.hpp"
    "include/project-management/integrity-check/version-integrity-checker.hpp"
)

set(PRJ_MGMT_SOURCE_FILES
    "src/integrity-check/version-integrity-checker.cpp"
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

if(USE_SEMVER)
    target_link_libraries(project_management_static INTERFACE semver_header_only)
    target_include_directories(project_management_static PUBLIC "${PROJECT_SOURCE_DIR}/third-party/semver/include")
endif()
