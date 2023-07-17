// Copyright (c) 2023 Andrea Ballestrazzi
#include <gc-project/project-controller.hpp>

namespace rpi_gc::gc_project {

    void ProjectController::setCurrentProject(const project_type& newProject) {
        // We need to close the old project before setting the new one.
        close_current_project();
        m_currentProject = newProject;
        m_currentProjectFilePath = std::filesystem::path{m_currentProject.value().getTitle() + ".json"};
    }

    void ProjectController::close_current_project() noexcept {
        // For now there isn't any complex behavior to do here.
        // We simply set the project.
        m_currentProject = std::nullopt;
        m_currentProjectFilePath.clear();
    }

} // namespace rpi_gc::gc_project
