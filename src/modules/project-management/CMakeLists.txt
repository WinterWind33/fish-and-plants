set(PRJ_MGMT_INCLUDE_ABS_DIR "${PROJECT_SOURCE_DIR}/src/modules/project-management/include")

set(PRJ_MGMT_INCLUDE_FILES
    "include/project-management/project.hpp"
)

add_library(project_management_static STATIC "src/temp.cpp")
set_target_properties(project_management_static
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
    LINKER_LANGUAGE CXX
)

target_include_directories(project_management_static PUBLIC ${PRJ_MGMT_INCLUDE_ABS_DIR})
