// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/project.hpp>

// C++ STL
#include <optional>

namespace gc::project_management::integrity_check {

    //!!
    //! \brief Represents the basic interface of a project integrity checker.
    //!  A project integrity checker is responsible to check if the given project
    //!  has all the correct fields to its proper functioning and eventually
    //!  try to apply the fixes.
    //!  Whenever the fixes cannot be applied, an exception can be thrown.
    struct ProjectIntegrityChecker {
        virtual ~ProjectIntegrityChecker() noexcept = default;

        //!!
        //! \brief Checks the integrity of the given project.
        //!
        //! \param project The project whose integrity is to be checked.
        //! \return True is the integrity is fine, false otherwise.
        [[nodiscard]]
        virtual bool checkIntegrity(const Project& project) const noexcept = 0;

        //!!
        //! \brief Tries to apply eventual fixes to all the errors in the given project.
        //!  Does not check if the integrity is full or not, it goes to all the fields
        //!  and tries to apply the correct values.
        //!  If the project cannot be fixed, the implementer may throw an exception.
        //!
        //! \param project The project whose integrity is to be restored.
        //! \return True if the fixes were fully applied, false otherwise.
        [[nodiscard]]
        virtual bool tryApplyIntegrityFixes(Project& project) = 0;
    } ;

} // namespace project_management::integrity_check
