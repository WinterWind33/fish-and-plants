// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/project.hpp>

// C++ STL
#include <filesystem>
#include <memory>

namespace gc::project_management::project_io {

//!!
//! \brief Represents the interface of a project writer. This serializes or
//!  sends the project structure to the desired sink.
//!
struct ProjectWriter {
    virtual ~ProjectWriter() noexcept = default;

    //!!
    //! \brief Serializes the project. Can throw exceptions if the serialization fails.
    //!
    //! \param project The project to serialize.
    virtual void serializeProject(const Project& project) = 0;
};

inline ProjectWriter& operator<<(ProjectWriter& writer, const Project& project) {
    writer.serializeProject(project);
    return writer;
}

//!!
//! \brief Create a Json Project File Writer object with the specified path. If the directories
//!  in the path don't exist, it will create them before creating the new file.
//!
//! \param outputFilePath The file path of the json file.
//! \return A new project writer ready to serialize the project.
std::unique_ptr<ProjectWriter> createJsonProjectFileWriter(
    const std::filesystem::path& outputFilePath);

} // namespace gc::project_management::project_io
