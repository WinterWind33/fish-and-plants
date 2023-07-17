// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/project.hpp>

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
        virtual Project readProject() = 0;
    };

    inline ProjectReader& operator>>(ProjectReader& reader, Project& inputProject) {
        inputProject = reader.readProject();
        return reader;
    }

} // namespace gc::project_management::project_io
