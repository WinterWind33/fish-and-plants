// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <gmock/gmock.h>
#include <gc-project/project-component.hpp>

namespace rpi_gc::gc_project::mocks {

class ProjectComponentMock : public ProjectComponent {
public:
    MOCK_METHOD(void, saveToProject, (gc::project_management::Project&), (final));
    MOCK_METHOD(void, loadConfigFromProject, (const gc::project_management::Project&), (final));
};

} // namespace rpi_gc::gc_project::mocks
