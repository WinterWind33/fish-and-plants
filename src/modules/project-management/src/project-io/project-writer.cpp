// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-management/project-io/project-writer.hpp>

#include <project-io/json-project-writer.hpp>

// C++ STL
#include <fstream>

namespace gc::project_management::project_io {

    std::unique_ptr<ProjectWriter> createJsonProjectFileWriter(const std::filesystem::path& filePath) {
        // We need to create the directories if they don't exist.
        std::filesystem::create_directories(filePath.parent_path());

        auto outputJsonFile{std::make_unique<std::ofstream>(filePath)};

        return std::make_unique<JsonProjectWriter>(std::move(outputJsonFile));
    }

} // namespace gc::project_management::project_io
