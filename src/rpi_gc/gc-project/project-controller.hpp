// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <gc-project/project-component.hpp>

#include <project-management/project.hpp>

// C++ STL
#include <optional>
#include <filesystem>
#include <vector>
#include <functional>

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
        void setCurrentProject(project_type&& project);

        void setCurrentProjectFilePath(std::filesystem::path filepath) noexcept {
            m_currentProjectFilePath = std::move(filepath);
        }

        void collectProjectData();
        void loadProjectData();

        void registerProjectComponent(ProjectComponent& projectComponent) {
            m_projectComponents.push_back(std::ref(projectComponent));
        }

        [[nodiscard]]
        const auto& getCurrentProjectFilePath() const noexcept {
            return m_currentProjectFilePath;
        }

    private:
        void close_current_project() noexcept;
        std::optional<project_type> m_currentProject{};
        std::filesystem::path m_currentProjectFilePath{};
        std::vector<std::reference_wrapper<ProjectComponent>> m_projectComponents{};
    };

} // namespace rpi_gc::gc_project
