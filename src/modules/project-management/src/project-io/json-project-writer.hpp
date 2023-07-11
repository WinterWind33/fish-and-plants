// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include "project-management/project-io/project-writer.hpp"

namespace gc::project_management::project_io {

    class JsonProjectWriter final : public ProjectWriter {
    public:

        void serializeProject(const Project& project) override;

    private:
    };

} // namespace gc::project_management::project_io
