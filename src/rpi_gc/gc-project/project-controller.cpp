// Copyright (c) 2023 Andrea Ballestrazzi
#include <gc-project/project-controller.hpp>

namespace rpi_gc::gc_project {

    void ProjectController::setCurrentProject(project_type newProject) {
        // We need to close the old project before setting the new one.
        close_current_project();
        m_currentProject = newProject;
    }

    void ProjectController::close_current_project() noexcept {
        // For now there isn't any complex behavior to do here.
        // We simply set the project.
        m_currentProject = std::nullopt;
    }


} // namespace rpi_gc::gc_project
