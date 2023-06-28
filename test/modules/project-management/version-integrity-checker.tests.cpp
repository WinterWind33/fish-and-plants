// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-management/integrity-check/version-integrity-checker.hpp>

#include <testing-core.hpp>

TEST_CASE("VersionIntegrityChecker static tests", "[static][modules][project-management][integrity-check][VersionIntegrityChecker]") {
    using namespace gc::project_management;

    constexpr semver::version OLD_VERSION{1, 0, 0};
    constexpr semver::version NEW_VERSION{2, 0, 0};
    constexpr Project OLD_PROJECT{Project::time_point_type{}, "TestProject", OLD_VERSION};
    constexpr integrity_check::VersionIntegrityChecker CHECKER_UNDER_TEST{NEW_VERSION};

    STATIC_CHECK_FALSE(CHECKER_UNDER_TEST.checkIntegrity(OLD_PROJECT));
}
