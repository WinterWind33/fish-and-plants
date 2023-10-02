// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-loader.hpp>

// Upgraders
#include <gc-project/upgraders/project-upgraders.hpp>

#include <version/version-numbers.hpp>

#include <project-management/integrity-check/title-integrity-checker.hpp>
#include <project-management/integrity-check/version-integrity-checker.hpp>
#include <project-management/project-io/project-reader.hpp>

namespace rpi_gc {

std::optional<gc::project_management::Project> LoadProjectAndCheckIntegrity(
    const std::filesystem::path& projectPath, gh_log::Logger& logger) noexcept {
    using namespace gc::project_management;

    // If the file doesn't exists, we log an error and return
    // an empty optional.
    if (!std::filesystem::exists(projectPath)) {
        logger.logError("The project file doesn't exists");
        return std::nullopt;
    }

    logger.logInfo("Loading project from path: " + projectPath.string());

    // Load the project from the given path.
    auto projectReader{
        gc::project_management::project_io::CreateJsonProjectFileReader(projectPath)};

    Project project{};
    *projectReader >> project;

    integrity_check::TitleIntegrityChecker titleIntegrityChecker{"unknown-project"};
    // If the project title need an integrity update we perform it.
    if (!titleIntegrityChecker.checkIntegrity(project)) {
        [[maybe_unused]] const bool bRes{titleIntegrityChecker.tryApplyIntegrityFixes(project)};
    }

    integrity_check::VersionIntegrityChecker versionIntegrityChecker{
        version::getApplicationVersion()};
    // If the project version need an integrity update we perform it.
    if (!versionIntegrityChecker.checkIntegrity(project)) {
        [[maybe_unused]] const bool bRes{versionIntegrityChecker.tryApplyIntegrityFixes(project)};
    }

    // Custom upgraders
    std::array<std::unique_ptr<integrity_check::ProjectIntegrityChecker>, 1> upgraders{
        std::make_unique<gc_project::upgraders::ProjectUpgrader_V1_1ToV1_2>()};

    // Apply the upgraders.
    for (auto& upgrader : upgraders) {
        if (!upgrader->checkIntegrity(project)) {
            [[maybe_unused]] const bool bRes{upgrader->tryApplyIntegrityFixes(project)};
        }
    }

    return project;
}

} // namespace rpi_gc
