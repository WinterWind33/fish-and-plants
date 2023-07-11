// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/project.hpp>

// C++ STL
#include <memory>
#include <filesystem>

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

    std::unique_ptr<ProjectWriter> createJsonProjectFileWriter(std::filesystem::path outputFilePath);

} // namespace gc::project_management::project_io
