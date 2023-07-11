// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include "project-management/project-io/project-writer.hpp"

// C++ STL
#include <ostream>
#include <functional> // for std::reference_wrapper

namespace gc::project_management::project_io {

    class JsonProjectWriter final : public ProjectWriter {
    public:
        explicit JsonProjectWriter(std::ostream& outputStream) noexcept;

        void serializeProject(const Project& project) override;

    private:
        std::reference_wrapper<std::ostream> m_outputStream;
    };

} // namespace gc::project_management::project_io
