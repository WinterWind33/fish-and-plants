// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/project.hpp>

// C++ STL
#include <filesystem>

namespace gc::project_management::project_io {

    //!!
    //! \brief Reads a project from an input source.
    //!
    struct ProjectReader {
        virtual ~ProjectReader() noexcept = default;

        //!!
        //! \brief Reads the project associated with this reader.
        //!
        //! \return Project The project to be read.
        [[nodiscard]]
        virtual Project readProject() = 0;
    };

    inline ProjectReader& operator>>(ProjectReader& reader, Project& inputProject) {
        inputProject = reader.readProject();
        return reader;
    }

    //!!
    //! \brief Create a Json Project File Reader object with the specified path.
    //!  If the file doesn't exist it throws an std::system_error.
    //! \param path
    //! \return std::unique_ptr<ProjectReader>
    [[nodiscard]] std::unique_ptr<ProjectReader> CreateJsonProjectFileReader(const std::filesystem::path& path);

} // namespace gc::project_management::project_io
