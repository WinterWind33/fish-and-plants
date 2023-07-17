// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/project.hpp>

// C++ STL
#include <istream>

namespace gc::project_management::project_io {

    class JsonProjectReader final {
    public:
        explicit JsonProjectReader(std::istream& inputStream) noexcept;

        Project readProject();

    private:
        std::istream& m_inputStream;
    };

    inline JsonProjectReader& operator>>(JsonProjectReader& reader, Project& inputProject) {
        inputProject = reader.readProject();
        return reader;
    }

} // namespace gc::project_management::project_io
