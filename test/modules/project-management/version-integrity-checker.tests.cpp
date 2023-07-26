// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-management/integrity-check/version-integrity-checker.hpp>

#include <testing-core.hpp>

TEST_CASE(
    "VersionIntegrityChecker unit tests",
    "[unit][solitary][modules][project-management][integrity-check][VersionIntegrityChecker]") {
    using namespace gc::project_management;

    constexpr semver::version OLD_VERSION{1, 0, 0};
    constexpr semver::version NEW_VERSION{2, 0, 0};

    integrity_check::VersionIntegrityChecker checkUnderTest{NEW_VERSION};

    GIVEN("A project with an old version") {
        Project project{Project::time_point_type{}, "TestProject", OLD_VERSION};

        THEN("The integrity check should fail") {
            CHECK_FALSE(checkUnderTest.checkIntegrity(project));
        }

        WHEN("The project version is updated") {
            const bool bRes{checkUnderTest.tryApplyIntegrityFixes(project)};

            THEN("The project version should be updated") {
                CHECK(project.getVersion() == NEW_VERSION);
            }

            THEN("The execution should succeed") {
                CHECK(bRes);
            }
        }
    }

    GIVEN("A project with the same version of the checker") {
        Project project{Project::time_point_type{}, "TestProject", NEW_VERSION};

        THEN("The integrity check should pass") {
            CHECK(checkUnderTest.checkIntegrity(project));
        }

        WHEN("The project version is updated") {
            const bool bRes{checkUnderTest.tryApplyIntegrityFixes(project)};

            THEN("The project version should be the same") {
                CHECK(project.getVersion() == NEW_VERSION);
            }

            THEN("The execution should succeed") {
                CHECK(bRes);
            }
        }
    }
}
