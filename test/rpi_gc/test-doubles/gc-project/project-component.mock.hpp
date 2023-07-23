// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <gc-project/project-component.hpp>
#include <gmock/gmock.h>

namespace rpi_gc::gc_project::mocks {

    class ProjectComponentMock : public ProjectComponent {
    public:
        MOCK_METHOD(void, saveToProject, (gc::project_management::Project&), (final));
        MOCK_METHOD(void, loadConfigFromProject, (const gc::project_management::Project&), (final));
    };

} // namespace rpi_gc::gc_project::mocks
