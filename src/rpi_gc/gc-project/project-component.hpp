// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/project.hpp>

namespace rpi_gc::gc_project {

//!!
//! \brief Interface implemented by all of that stateful classes that
//!  can or need to save data into the Greenhouse CAD project.
struct ProjectComponent {
    virtual ~ProjectComponent() noexcept = default;

    //!!
    //! \brief Saves the component data inside the given project.
    //!
    //! \param project The project which will receive the component data.
    virtual void saveToProject(gc::project_management::Project& project) = 0;

    //!!
    //! \brief Loads the component data from the given project if there is one.
    //!  If there is no component data inside the project it's up to the realization to
    //!  decide whether to throw an error or not.
    //!
    //! \param project The project from which the component data will be loaded.
    virtual void loadConfigFromProject(const gc::project_management::Project& project) = 0;
};

} // namespace rpi_gc::gc_project
