// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/project-io/project-reader.hpp>
#include <project-management/project.hpp>

// C++ STL
#include <istream>
#include <memory>

namespace gc::project_management::project_io {

    class JsonProjectReader final : public ProjectReader {
    public:
        explicit JsonProjectReader(std::unique_ptr<std::istream> inputStream) noexcept;

        [[nodiscard]] Project readProject() override;

    private:
        std::unique_ptr<std::istream> m_inputStream;
    };

} // namespace gc::project_management::project_io
