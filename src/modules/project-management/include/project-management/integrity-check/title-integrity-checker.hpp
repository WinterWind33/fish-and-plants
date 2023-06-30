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


        constexpr bool checkIntegrity(const Project& project) const noexcept override {
            return !project.getTitle().empty();
        }

        bool tryApplyIntegrityFixes(Project& project) override {
            project = Project(project.getCreationTime(), m_defaultTitle, project.getVersion());

            return true;
        }

    private:
        Project::project_title m_defaultTitle{};
    };

} // namespace gc::project_management::integrity_check
