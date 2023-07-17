// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-management/project-io/project-reader.hpp>

namespace gc::project_management::project_io {

std::unique_ptr<ProjectReader> CreateJsonProjectFileReader(const std::filesystem::path& path) {
    return nullptr;
}

} // namespace gc::project_management::project_io
