// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-management/integrity-check/title-integrity-checker.hpp>

#include <testing-core.hpp>

TEST_CASE("TitleIntegrityChecker unit tests", "[unit][solitary][modules][project-management][integrity-check][TitleIntegrityChecker]") {
    using namespace gc::project_management;

    constexpr std::string_view EXPECTED_TITLE{"DefaultTitle"};
    integrity_check::TitleIntegrityChecker checkerUnderTest{EXPECTED_TITLE};

    GIVEN("A project without a title") {
        Project project{Project::time_point_type{}, "", semver::version{1, 0, 0}};

        THEN("The integrity check should fail") {
            CHECK_FALSE(checkerUnderTest.checkIntegrity(project));
        }

        WHEN("The title is updated") {
            const bool bRes{checkerUnderTest.tryApplyIntegrityFixes(project)};

            THEN("The title should be the default one") {
                CHECK(project.getTitle() == EXPECTED_TITLE);
            }

            THEN("The execution should succeed") {
                CHECK(bRes);
            }
        }
    }

    GIVEN("A project with a title") {
        constexpr std::string_view VALID_TITLE{"ValidTitle"};
        Project project{Project::time_point_type{}, VALID_TITLE, semver::version{1, 0, 0}};

        THEN("The integrity check should pass") {
            CHECK(checkerUnderTest.checkIntegrity(project));
        }

        WHEN("The title is updated") {
            const bool bRes{checkerUnderTest.tryApplyIntegrityFixes(project)};

            THEN("The title should be the default one") {
                CHECK(project.getTitle() == EXPECTED_TITLE);
            }

            THEN("The execution should succeed") {
                CHECK(bRes);
            }
        }
    }
}
