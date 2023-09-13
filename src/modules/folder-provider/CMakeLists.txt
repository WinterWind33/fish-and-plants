# Copyright (c) 2023 Andrea Ballestrazzi

set(GC_FOLDER_PROVIDER_SOURCE_FILES
    "src/folder-provider.cpp"
    "src/folder-provider-impl.cpp"
)
set(GC_FOLDER_PROVIDER_HEADER_FILES
    "include/folder-provider/folder-provider.hpp"
    "src/folder-provider-impl.hpp"
)

add_library(gc_folder_provider STATIC ${GC_FOLDER_PROVIDER_SOURCE_FILES} ${GC_FOLDER_PROVIDER_HEADER_FILES})
target_include_directories(gc_folder_provider PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(gc_folder_provider PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_target_properties(gc_folder_provider
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PRODUCTION_LIB_COMPILATION_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PRODUCTION_EXE_COMPILATION_OUTPUT_DIR}
)

if(GC_USE_SAGO_PLATFORM_FOLDERS)
    target_link_libraries(gc_folder_provider PRIVATE sago::platform_folders)
endif()
