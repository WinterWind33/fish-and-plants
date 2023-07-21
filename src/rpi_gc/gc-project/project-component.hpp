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
    };

} // namespace rpi_gc::gc_project
