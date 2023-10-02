// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <project-management/integrity-check/project-integrity-checker.hpp>
#include <project-management/project.hpp>

namespace rpi_gc::gc_project::upgraders {

class ProjectUpgrader_V1_1ToV1_2 final
    : public gc::project_management::integrity_check::ProjectIntegrityChecker {
public:
    [[nodiscard]] constexpr bool checkIntegrity(
        const gc::project_management::Project& project) const noexcept override {
        return false;
    }

    [[nodiscard]] bool tryApplyIntegrityFixes(gc::project_management::Project& project) override;
};

} // namespace rpi_gc::gc_project::upgraders
