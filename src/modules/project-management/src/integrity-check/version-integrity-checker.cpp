// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-management/integrity-check/version-integrity-checker.hpp>

namespace gc::project_management::integrity_check {

bool VersionIntegrityChecker::tryApplyIntegrityFixes(Project& project) {
    Project::updateVersion(project, m_newVersion);

    return true;
}

} // namespace gc::project_management::integrity_check
