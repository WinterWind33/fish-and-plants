// Copyright (c) 2023 Andrea ballestrazzi

#include <src/project-io/json-project-reader.hpp>
#include <project-management/project.hpp>

#include <testing-core.hpp>

// C++ STL
#include <sstream>
#include <chrono>

TEST_CASE("JsonProjectReader unit tests", "[unit][sociable][project-management][project-io][JsonProjectReader]") {
    using namespace gc::project_management;

    GIVEN("An project with invalid version") {
        // The time point represents the 01/01/2023 12:30:40 PM timedate.
        auto inputStream{std::make_unique<std::istringstream>(R"(
            {
                "creation_timedate": 1672576240,
                "title": "test-title",
                "version": ""
            }
        )")};

        constexpr semver::version EXPECTED_VERSION{0, 0, 0};

        project_io::JsonProjectReader projectReaderUnderTest{std::move(inputStream)};

        WHEN("The project is read") {
            Project inputProject{};
            projectReaderUnderTest >> inputProject;

            THEN("The read project should have a zeroed version") {
                CHECK(inputProject.getVersion() == EXPECTED_VERSION);
            }
        }
    }

    GIVEN("A trivial project") {
        // The time point represents the 01/01/2023 12:30:40 PM timedate.
        auto inputStream{std::make_unique<std::istringstream>(R"(
            {
                "creation_timedate": 1672576240,
                "title": "test-title",
                "version": "1.2.3"
            }
        )")};

        const Project expectedProject{std::chrono::system_clock::from_time_t(1672576240), "test-title", semver::version{1, 2, 3}};

        project_io::JsonProjectReader projectReaderUnderTest{std::move(inputStream)};

        WHEN("The project is read") {
            Project inputProject{};
            projectReaderUnderTest >> inputProject;

            THEN("The read project should be correct") {
                CHECK(inputProject == expectedProject);
            }
        }
    }
}
