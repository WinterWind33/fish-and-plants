# Copyright (c) 2022 Andrea Ballestrazzi

set(GH_CMD_HEADER_FILES)

add_library(gh_cmd INTERFACE ${GH_CMD_HEADER_FILES})
target_include_directories(gh_cmd INTERFACE "${PROJECT_SOURCE_DIR}/src/wrappers/gh_cmd")

# Dependencies
if(USE_POPL20)
    target_link_libraries(gh_cmd INTERFACE popl20)
    target_compile_definitions(gh_cmd INTERFACE "USE_POPL20")
    target_include_directories(gh_cmd INTERFACE "${PROJECT_SOURCE_DIR}/src/third-party/popl20")
endif()
