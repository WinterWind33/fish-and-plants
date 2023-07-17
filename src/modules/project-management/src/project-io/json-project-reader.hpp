// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/project.hpp>

// C++ STL
#include <istream>

namespace gc::project_management::project_io {

    class JsonProjectReader final {
    public:
        explicit JsonProjectReader(std::istream& inputStream) noexcept;

    private:
        std::istream& m_inputStream;
    };

    inline JsonProjectReader& operator>>(JsonProjectReader& reader, Project& inputProject) {
        return reader;
    }

} // namespace gc::project_management::project_io
