# Copyright (c) 2023 Andrea Ballestrazzi

set(FEP_WORKFLOWS_HEADER_FILES
    "include/workflows/workflow.hpp"
)

set(FEP_WORKFLOWS_SOURCE_FILES
    "src/workflow.cpp"
)

add_library(fep_workflows STATIC ${FEP_WORKFLOWS_HEADER_FILES} ${FEP_WORKFLOWS_SOURCE_FILES})

# Set the output directories
set_target_properties(fep_workflows
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
)

# Set the include directories
target_include_directories(fep_workflows PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(fep_workflows PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
