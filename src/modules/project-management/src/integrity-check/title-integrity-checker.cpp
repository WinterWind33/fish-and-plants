// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-management/integrity-check/title-integrity-checker.hpp>

namespace gc::project_management::integrity_check {

    bool TitleIntegrityChecker::tryApplyIntegrityFixes(Project& project)  {
        project = Project(project.getCreationTime(), m_defaultTitle, project.getVersion());

        return true;
    }

} // namespace gc::project_management::integrity_check
