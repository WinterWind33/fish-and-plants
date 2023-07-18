// Copyright (c) 2023 Andrea Ballestrazzi

#include <project-management/project.hpp>

#include <testing-core.hpp>

TEST_CASE("Project fields static tests", "[static][modules][project-management][ProjectFields]") {
    using namespace gc::project_management;

    STATIC_CHECK(project_fields::ProjectValueType<bool>);
    STATIC_CHECK(project_fields::ProjectValueType<std::int32_t>);
    STATIC_CHECK(project_fields::ProjectValueType<std::string>);
}

TEST_CASE("Project unit tests", "[unit][sociable][modules][project-management][Project]") {
    using namespace gc::project_management;

    GIVEN("A simple project") {
        Project projectUnderTest{Project::time_point_type{}, "test-title", semver::version{}};

        WHEN("A std::string value is added") {
            projectUnderTest.addValueField(std::string{"test-key"}, std::string{"test-value"});

            THEN("The project structure should have the value") {
                const auto& projectStructure{projectUnderTest.getStructure()};

                CHECK(projectStructure.Values.contains("test-key"));
            }
        }
    }
}
