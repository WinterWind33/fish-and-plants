// Copyright (c) 2023 Andrea Ballestrazzi
#include <src/project-io/json-project-writer.hpp>

#include <testing-core.hpp>
#include <nlohmann/json.hpp>

// C++ STL
#include <sstream>
#include <string_view>
#include <iostream>

namespace tests {

    nlohmann::json readJsonFromString(const std::string& str) {
        nlohmann::json actualJson{};
        std::istringstream ist{str};

        ist >> actualJson;

        return actualJson;
    }

    void checkBasicProjectData(const nlohmann::json& actualJson, const gc::project_management::Project& expectedProjectData) {
        REQUIRE(actualJson.contains("title"));
        CHECK(actualJson["title"] == expectedProjectData.getTitle());

        REQUIRE(actualJson.contains("version"));
        CHECK(actualJson["version"] == expectedProjectData.getVersion().to_string());

        REQUIRE(actualJson.contains("creation_timedate"));
        CHECK(actualJson["creation_timedate"] == std::chrono::system_clock::to_time_t(expectedProjectData.getCreationTime()));
    }

    template<typename T>
    T createGenericValue() noexcept {
        if constexpr (std::is_same_v<T, bool>)
            return false;
        else if constexpr (std::is_same_v<T, std::int32_t>)
            return 42;
        else if constexpr (std::is_same_v<T, double>)
            return -56.0;
        else if constexpr (std::is_same_v<T, std::string>)
            return "temp-value";

        return T{};
    }

} // namespace tests

TEST_CASE("JsonProjectWriter unit tests", "[unit][sociable][modules][project-management][JsonProjectWriter]") {
    using namespace gc::project_management;
    using namespace gc::project_management::project_io;

    auto outStream = std::make_unique<std::ostringstream>();
    auto& outStreamRef{*outStream};

    GIVEN("A JSON writer") {
        JsonProjectWriter writerUnderTest{std::move(outStream)};

        WHEN("A trivial project is serialized") {
            // Here "trivial" means a project without fields.
            const Project trivialProject{Project::time_point_type{}, "test-title", semver::version{1, 2, 3}};

            writerUnderTest << trivialProject;

            THEN("The final JSON should be correct") {
                nlohmann::json actualJson = tests::readJsonFromString(outStreamRef.str());

                tests::checkBasicProjectData(actualJson, trivialProject);
            }
        }
    }
}

TEMPLATE_TEST_CASE("JsonProjectWriter JSON formatting unit tests", "[unit][sociable][modules][project-management][JsonProjectWriter][JSON-Formatting]",
    std::int32_t, double, bool, std::string) {
    using namespace gc::project_management;
    using namespace gc::project_management::project_io;

    auto outStream = std::make_unique<std::ostringstream>();
    auto& outStreamRef{*outStream};

    GIVEN("A JSON writer") {
        JsonProjectWriter writerUnderTest{std::move(outStream)};

        constexpr std::string_view PROJECT_TITLE{"test-title"};
        constexpr semver::version PROJECT_VERSION{1, 2, 3};

        Project projectToWrite{Project::time_point_type{}, std::string{PROJECT_TITLE}, PROJECT_VERSION};

        AND_GIVEN("A project with a value field") {
            constexpr std::string_view FIELD_KEY{"test-key"};
            const TestType fieldValue{tests::createGenericValue<TestType>()};
            projectToWrite.addValue(FIELD_KEY.data(), fieldValue);

            WHEN("The project is serialized") {
                writerUnderTest << projectToWrite;

                THEN("It should have all the correct trivial fields") {
                    nlohmann::json actualJson = tests::readJsonFromString(outStreamRef.str());

                    tests::checkBasicProjectData(actualJson, projectToWrite);
                }

                THEN("It should have the field in the root node") {
                    nlohmann::json actualJson = tests::readJsonFromString(outStreamRef.str());

                    REQUIRE(actualJson.contains(FIELD_KEY));
                    CHECK(actualJson[FIELD_KEY].get<TestType>() == fieldValue);
                }
            }
        }

        AND_GIVEN("A project with a value array") {
            constexpr std::string_view FIELD_KEY{"test-key"};
            const std::vector<TestType> values{
                tests::createGenericValue<TestType>(),
                tests::createGenericValue<TestType>(),
                tests::createGenericValue<TestType>(),
                tests::createGenericValue<TestType>()
            };

            projectToWrite.addValueArray(FIELD_KEY.data(), values);

            WHEN("The project is serialized") {
                writerUnderTest << projectToWrite;

                THEN("It should have the field") {
                    nlohmann::json actualJson = tests::readJsonFromString(outStreamRef.str());

                    CHECK(actualJson.contains(FIELD_KEY));
                }

                THEN("It should have the field with the correct array values") {
                    nlohmann::json actualJson = tests::readJsonFromString(outStreamRef.str());

                    const TestType expectedValue{tests::createGenericValue<TestType>()};

                    for(const auto& val : actualJson.at(FIELD_KEY.data())) {
                        CHECK(val.get<TestType>() == expectedValue);
                    }
                }
            }
        }
    }
}
