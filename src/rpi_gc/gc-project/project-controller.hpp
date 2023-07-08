// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/project.hpp>

// C++ STL
#include <optional>

namespace rpi_gc::gc_project {

    class ProjectController final {
    public:
        using project_type = gc::project_management::Project;

        [[nodiscard]]
        bool hasProject() const noexcept {
            return m_currentProject.has_value();
        }

        [[nodiscard]]
        const project_type& getCurrentProject() const {
            return m_currentProject.value();
        }

        void setCurrentProject(project_type project);
        void closeCurrentProject() noexcept;

    private:
        std::optional<project_type> m_currentProject{};
    };

} // namespace rpi_gc::gc_project
