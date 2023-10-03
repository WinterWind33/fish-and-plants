// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/integrity-check/project-integrity-checker.hpp>
#include <project-management/project.hpp>

namespace rpi_gc::gc_project::upgraders {

//!!
//! \brief Project upgrader from version 1.1 to version 1.2.
//!  This upgrader changes the automatic watering system board pings configurations
//!  from a simple value to a full object with the activation state.
class ProjectUpgrader_V1_1ToV1_2 final
    : public gc::project_management::integrity_check::ProjectIntegrityChecker {
public:
    //!!
    //! \brief Check the integrity of the given project.
    //!
    //! \param project The project to check.
    //! \return True if the project has a version >= 1.2.
    //!
    [[nodiscard]] bool checkIntegrity(
        const gc::project_management::Project& project) const noexcept override;

    //!!
    //! \brief Try to apply the integrity fixes to the given project.
    //!  Tries to add the new objects for the automatic watering system board pins.
    //!
    //! \param project The project to fix.
    //! \return True if the project has been successfully upgraded, false otherwise.
    //!
    [[nodiscard]] bool tryApplyIntegrityFixes(gc::project_management::Project& project) override;
};

} // namespace rpi_gc::gc_project::upgraders
