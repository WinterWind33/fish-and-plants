// Copyright (c) 2023 Andrea Ballestrazzi
#include <project-management/project.hpp>

#include <testing-core.hpp>

TEST_CASE("Project static tests", "[static][modules][project-management][Project]") {
    using namespace gc::project_management;

    constexpr std::string_view PROJECT_TITLE{"ProjectTitle"};
    constexpr Project projectUnderTest{
        Project::time_point_type{},
        PROJECT_TITLE,
        Project::project_version{1, 0, 0}
    };

    STATIC_CHECK(projectUnderTest.getCreationTime() == Project::time_point_type{});
    STATIC_CHECK(projectUnderTest.getTitle() == PROJECT_TITLE);
    STATIC_CHECK(projectUnderTest.getVersion() == Project::project_version{1, 0, 0});
}
