// Copyright (c) 2023 Andrea ballestrazzi

#include <project-management/project-io/project-reader.hpp>
#include <project-management/project.hpp>

#include <testing-core.hpp>

// C++ STL
#include <sstream>
#include <chrono>

TEST_CASE("JsonProjectReader unit tests", "[unit][sociable][project-management][project-io][JsonProjectReader]") {
    using namespace gc::project_management;

    // The time point represents the 01/01/2023 12:30:40 PM timedate.
    std::istringstream inputStream{R"(
        {
            "creation_timedate": "1672576240",
            "title": "test-title",
            "version": "1.2.3"
        }
    )"};

    const Project expectedProject{std::chrono::system_clock::from_time_t(1672576240), "test-title", semver::version{1, 2, 3}};

    project_io::JsonProjectReader projectReaderUnderTest{inputStream};

    SECTION("Should correctly read the projct") {
        Project inputProject{};
        projectReaderUnderTest >> inputProject;

        CHECK(inputProject == expectedProject);
    }
}
