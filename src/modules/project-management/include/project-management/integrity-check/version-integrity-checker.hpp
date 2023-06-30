// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/integrity-check/project-integrity-checker.hpp>

// Third-party
#include <semver.hpp>

namespace gc::project_management::integrity_check {

    class VersionIntegrityChecker final : public ProjectIntegrityChecker {
    public:
        constexpr explicit VersionIntegrityChecker(semver::version version) noexcept :
            m_newVersion{version} {}

        constexpr bool checkIntegrity(const Project& project) const noexcept override {
            return m_newVersion == project.getVersion();
        }

        bool tryApplyIntegrityFixes(Project& project) override;

    private:
        const semver::version m_newVersion;
    };

} // namespace project_management::integrity_check
