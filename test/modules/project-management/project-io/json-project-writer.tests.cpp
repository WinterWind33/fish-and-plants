// Copyright (c) 2023 Andrea Ballestrazzi
#include <src/project-io/json-project-writer.hpp>

#include <testing-core.hpp>
#include <nlohmann/json.hpp>

// C++ STL
#include <sstream>

TEST_CASE("JsonProjectWriter unit tests", "[unit][sociable][modules][project-management][JsonProjectWriter]") {
    using namespace gc::project_management;
    using namespace gc::project_management::project_io;

    auto outStream = std::make_unique<std::ostringstream>();
    auto& outStreamRef{*outStream};

    GIVEN("A JSON writer") {
        JsonProjectWriter writerUnderTest{std::move(outStream)};

        WHEN("A trivial project is serialized") {
            // Here "trivial" means a project without fields.
            Project trivialProject{Project::time_point_type{}, "test-title", semver::version{1, 2, 3}};

            writerUnderTest << trivialProject;

            THEN("The final JSON should be correct") {
                nlohmann::json actualJson{};
                std::istringstream ist{outStreamRef.str()};

                REQUIRE_NOTHROW(ist >> actualJson);

                CHECK(actualJson["title"] == "test-title");
                CHECK(actualJson["version"] == "1.2.3");
                CHECK(actualJson["creation_timedate"] == 0);
            }
        }
    }
}
