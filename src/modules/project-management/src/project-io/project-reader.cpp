// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-management/project-io/project-reader.hpp>

// C++ STL
#include <stdexcept>

namespace gc::project_management::project_io {

std::unique_ptr<ProjectReader> CreateJsonProjectFileReader(const std::filesystem::path& path) {
    if(!std::filesystem::exists(path))
        throw std::invalid_argument{"The specified path does not exist."};

    if(!std::filesystem::is_regular_file(path))
        throw std::invalid_argument{"The specified path is not a valid JSON file."};

    return nullptr;
}

} // namespace gc::project_management::project_io
