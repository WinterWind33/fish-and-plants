// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-management/integrity-check/title-integrity-checker.hpp>

#include <testing-core.hpp>

TEST_CASE("TitleIntegrityChecker static tests", "[static][modules][project-management][integrity-check][TitleIntegrityChecker]") {
    using namespace gc::project_management;

    constexpr Project TEST_PROJECT{Project::time_point_type{}, "", semver::version{1,0,0}};
    constexpr std::string_view EXPECTED_TITLE{"DefaultTitle"};
    constexpr integrity_check::TitleIntegrityChecker checkerUnderTest{EXPECTED_TITLE};

    STATIC_CHECK_FALSE(checkerUnderTest.checkIntegrity(TEST_PROJECT));
}
