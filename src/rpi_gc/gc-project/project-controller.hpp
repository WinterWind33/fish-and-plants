// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/project.hpp>

// C++ STL
#include <optional>

namespace rpi_gc::gc_project {

    //!!
    //! \brief Controller of a Greenhouse CAD project.
    //!
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

        //!!
        //! \brief Sets the current project of this project controller. Closes
        //!  the previous one if set.
        //!
        //! \param project The new project to control.
        void setCurrentProject(project_type project);

    private:
        void close_current_project() noexcept;
        std::optional<project_type> m_currentProject{};
    };

} // namespace rpi_gc::gc_project
