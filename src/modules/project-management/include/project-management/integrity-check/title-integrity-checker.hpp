// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/integrity-check/project-integrity-checker.hpp>
#include <project-management/project.hpp>

// C++ STL
#include <string_view>

namespace gc::project_management::integrity_check {

    class TitleIntegrityChecker final : public ProjectIntegrityChecker {
    public:
        explicit constexpr TitleIntegrityChecker(Project::project_title defaultTitle) noexcept :
            m_defaultTitle{defaultTitle} {}


        [[nodiscard]]
        constexpr bool checkIntegrity(const Project& project) const noexcept override {
            return !project.getTitle().empty();
        }

        [[nodiscard]]
        bool tryApplyIntegrityFixes(Project& project) override;

    private:
        Project::project_title m_defaultTitle{};
    };

} // namespace gc::project_management::integrity_check
