// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/integrity-check/project-integrity-checker.hpp>
#include <project-management/project.hpp>

// C++ STL
#include <string_view>

namespace gc::project_management::integrity_check {

    //!!
    //! \brief Represents the integrity checker for the project's title.
    //!
    class TitleIntegrityChecker final : public ProjectIntegrityChecker {
    public:

        //!!
        //! \brief Constructs a new object with the given default title.
        //!
        explicit TitleIntegrityChecker(Project::project_title defaultTitle) noexcept :
            m_defaultTitle{std::move(defaultTitle)} {}

        //!!
        //! \brief Checks whether or not the given project has a valid title. This means that
        //!  the title shouldn't be empty.
        //! \param project The project whose title need to be checked.
        //! \return bool True if the project's title is OK, false otherwise.
        [[nodiscard]]
        bool checkIntegrity(const Project& project) const noexcept override {
            return !project.getTitle().empty();
        }


        //!!
        //! \brief Applies the default title to the given project, doesn't check if the
        //!  given project needs it or not.
        //!
        //! \param project The project whose title will be defaulted.
        //! \return bool True, always.
        [[nodiscard]]
        bool tryApplyIntegrityFixes(Project& project) override;

    private:
        Project::project_title m_defaultTitle{};
    };

} // namespace gc::project_management::integrity_check
