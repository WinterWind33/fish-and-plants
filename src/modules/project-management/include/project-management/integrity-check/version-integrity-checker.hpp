// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/integrity-check/project-integrity-checker.hpp>

// Third-party
#include <semver.hpp>

namespace gc::project_management::integrity_check {

//!!
//! \brief Represents the checker of the project version.
//!
class VersionIntegrityChecker final : public ProjectIntegrityChecker {
public:
    //!!
    //! \brief Constructs a new checker object with the given version. This version will
    //!  be used to check the project versions.
    constexpr explicit VersionIntegrityChecker(semver::version version) noexcept
        : m_newVersion{version} {}

    //!!
    //! \brief Checks whether or not the given project has the same version
    //!  of the default one.
    //!
    //! \param project The project whose version eed to be checked.
    //! \return True if the version is up to date, false otherwise.
    [[nodiscard]] constexpr bool checkIntegrity(const Project& project) const noexcept override {
        return m_newVersion == project.getVersion();
    }

    //!!
    //! \brief Applies the default version to the given project.
    //!
    //! \param project The project whose version need to be updated.
    //! \return bool True, always.
    [[nodiscard]] bool tryApplyIntegrityFixes(Project& project) override;

private:
    const semver::version m_newVersion;
};

} // namespace gc::project_management::integrity_check
