// Copyright (c) 2023 Andrea Ballestrazzi
#include <gc-project/project-controller.hpp>

#include <testing-core.hpp>

TEST_CASE("ProjectController unit tests", "[unit][sociable][rpi_gc][gc-project][ProjectController]") {
    using namespace rpi_gc::gc_project;

    ProjectController projectControllerUnderTest{};

    WHEN("A new project is set") {
        const gc::project_management::Project dummyProject{
            gc::project_management::Project::time_point_type{}, "DummyProject", semver::version{1, 0, 0}};

        projectControllerUnderTest.setCurrentProject(dummyProject);

        THEN("The default file path should be set") {
            CHECK(projectControllerUnderTest.getCurrentProjectFilePath() == "DummyProject.json");
        }
    }
}
