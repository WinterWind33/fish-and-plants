// Copyright (c) 2023 Andrea Ballestrazzi
#include <gc-project/project-controller.hpp>

// Test doubles
#include <rpi_gc/test-doubles/gc-project/project-component.mock.hpp>

#include <testing-core.hpp>

// C++ STL
#include <array>

TEST_CASE("ProjectController unit tests", "[unit][sociable][rpi_gc][gc-project][ProjectController]") {
    using namespace rpi_gc::gc_project;

    ProjectController projectControllerUnderTest{};

    WHEN("A new project is set") {
        gc::project_management::Project dummyProject{
            gc::project_management::Project::time_point_type{}, "DummyProject", semver::version{1, 0, 0}};

        projectControllerUnderTest.setCurrentProject(std::move(dummyProject));

        THEN("The default file path should be set") {
            CHECK(projectControllerUnderTest.getCurrentProjectFilePath() == "DummyProject.json");
        }
    }

    GIVEN("A project controller with a project") {
        using namespace gc::project_management;

        Project dummyProject{Project::time_point_type{}, "test-project", semver::version{1, 2, 3}};
        projectControllerUnderTest.setCurrentProject(std::move(dummyProject));
        const auto& expectedProject{projectControllerUnderTest.getCurrentProject()};

        WHEN("The project saving is triggered") {
            THEN("The project components all should be queried") {
                std::array<mocks::ProjectComponentMock, 2> projectComponents{};

                for(auto& comp : projectComponents) {
                    EXPECT_CALL(comp, saveToProject(::testing::Ref(expectedProject))).Times(1);

                    projectControllerUnderTest.registerProjectComponent(comp);
                }

                projectControllerUnderTest.collectProjectData();
            }
        }
    }

    GIVEN("A project controller without a project") {
        WHEN("The project saving is triggered") {
            THEN("No component should be queried") {
                std::array<mocks::ProjectComponentMock, 2> projectComponents{};

                for(auto& comp : projectComponents) {
                    EXPECT_CALL(comp, saveToProject).Times(0);

                    projectControllerUnderTest.registerProjectComponent(comp);
                }

                projectControllerUnderTest.collectProjectData();
            }
        }
    }
}
