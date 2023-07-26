// Copyright (c) 2023 Andrea Ballestrazzi
#include <gc-project/project-controller.hpp>

namespace rpi_gc::gc_project {

void ProjectController::setCurrentProject(project_type&& newProject) {
    // We need to close the old project before setting the new one.
    close_current_project();
    m_currentProject = std::move(newProject);
    m_currentProjectFilePath = std::filesystem::path{m_currentProject.value().getTitle() + ".json"};
}

void ProjectController::close_current_project() noexcept {
    // For now there isn't any complex behavior to do here.
    // We simply set the project.
    m_currentProject = std::nullopt;
    m_currentProjectFilePath.clear();
}

void ProjectController::collectProjectData() {
    if (!hasProject()) {
        return;
    }

    for (auto& comp : m_projectComponents) {
        comp.get().saveToProject(m_currentProject.value());
    }
}

void ProjectController::loadProjectData() {
    if (!hasProject()) {
        return;
    }

    for (auto& comp : m_projectComponents) {
        comp.get().loadConfigFromProject(m_currentProject.value());
    }
}

} // namespace rpi_gc::gc_project
