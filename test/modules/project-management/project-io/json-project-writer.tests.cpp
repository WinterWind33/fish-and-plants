// Copyright (c) 2023 Andrea Ballestrazzi
#include <src/project-io/json-project-writer.hpp>

#include <nlohmann/json.hpp>
#include <testing-core.hpp>

// C++ STL
#include <sstream>
#include <string_view>

namespace tests {

nlohmann::json readJsonFromString(const std::string& str) {
    nlohmann::json actualJson{};
    std::istringstream ist{str};

    ist >> actualJson;

    return actualJson;
}

void checkBasicProjectData(const nlohmann::json& actualJson,
                           const gc::project_management::Project& expectedProjectData) {
    REQUIRE(actualJson.contains("title"));
    CHECK(actualJson["title"] == expectedProjectData.getTitle());

    REQUIRE(actualJson.contains("version"));
    CHECK(actualJson["version"] == expectedProjectData.getVersion().to_string());

    REQUIRE(actualJson.contains("creation_timedate"));
    CHECK(actualJson["creation_timedate"] ==
          std::chrono::system_clock::to_time_t(expectedProjectData.getCreationTime()));
}

template <typename T>
T createGenericValue() noexcept {
    if constexpr (std::is_same_v<T, bool>)
        return false;
    else if constexpr (std::is_same_v<T, std::int64_t>)
        return 42;
    else if constexpr (std::is_same_v<T, double>)
        return -56.0;
    else if constexpr (std::is_same_v<T, std::string>)
        return "temp-value";

    return T{};
}

} // namespace tests

TEST_CASE("JsonProjectWriter unit tests",
          "[unit][sociable][modules][project-management][JsonProjectWriter]") {
    using namespace gc::project_management;
    using namespace gc::project_management::project_io;

    auto outStream = std::make_unique<std::ostringstream>();
    auto& outStreamRef{*outStream};

    GIVEN("A JSON writer") {
        JsonProjectWriter writerUnderTest{std::move(outStream)};

        WHEN("A trivial project is serialized") {
            // Here "trivial" means a project without fields.
            const Project trivialProject{
                Project::time_point_type{},
                "test-title", semver::version{1, 2, 3}
            };

            writerUnderTest << trivialProject;

            THEN("The final JSON should be correct") {
                const nlohmann::json actualJson = tests::readJsonFromString(outStreamRef.str());

                tests::checkBasicProjectData(actualJson, trivialProject);
            }
        }
    }
}

TEMPLATE_TEST_CASE(
    "JsonProjectWriter JSON formatting unit tests",
    "[unit][sociable][modules][project-management][JsonProjectWriter][JSON-Formatting]",
    std::int64_t, double, bool, std::string) {
    using namespace gc::project_management;
    using namespace gc::project_management::project_io;

    auto outStream = std::make_unique<std::ostringstream>();
    auto& outStreamRef{*outStream};

    GIVEN("A JSON writer") {
        JsonProjectWriter writerUnderTest{std::move(outStream)};

        constexpr std::string_view PROJECT_TITLE{"test-title"};
        constexpr semver::version PROJECT_VERSION{1, 2, 3};

        Project projectToWrite{Project::time_point_type{}, std::string{PROJECT_TITLE},
                               PROJECT_VERSION};

        AND_GIVEN("A project with a value field") {
            constexpr std::string_view FIELD_KEY{"test-key"};
            const TestType fieldValue{tests::createGenericValue<TestType>()};
            projectToWrite.addValue(FIELD_KEY.data(), fieldValue);

            WHEN("The project is serialized") {
                writerUnderTest << projectToWrite;

                THEN("It should have all the correct trivial fields") {
                    const nlohmann::json actualJson = tests::readJsonFromString(outStreamRef.str());

                    tests::checkBasicProjectData(actualJson, projectToWrite);
                }

                THEN("It should have the field in the root node") {
                    const nlohmann::json actualJson = tests::readJsonFromString(outStreamRef.str());

                    REQUIRE(actualJson.contains(FIELD_KEY));
                    CHECK(actualJson[FIELD_KEY].get<TestType>() == fieldValue);
                }
            }
        }

        AND_GIVEN("A project with a value array") {
            constexpr std::string_view FIELD_KEY{"test-key"};
            const std::vector<TestType> values{
                tests::createGenericValue<TestType>(), tests::createGenericValue<TestType>(),
                tests::createGenericValue<TestType>(), tests::createGenericValue<TestType>()};

            projectToWrite.addValueArray(FIELD_KEY.data(), values);

            WHEN("The project is serialized") {
                writerUnderTest << projectToWrite;

                THEN("It should have the field") {
                    const nlohmann::json actualJson = tests::readJsonFromString(outStreamRef.str());

                    CHECK(actualJson.contains(FIELD_KEY));
                }

                THEN("It should have the field with the correct array values") {
                    const nlohmann::json actualJson = tests::readJsonFromString(outStreamRef.str());

                    const TestType expectedValue{tests::createGenericValue<TestType>()};

                    for (const auto& val : actualJson.at(FIELD_KEY.data())) {
                        CHECK(val.get<TestType>() == expectedValue);
                    }
                }
            }
        }

        AND_GIVEN("A project with an object") {
            using namespace std::string_literals;
            constexpr std::string_view FIELD_KEY{"test-key"};
            constexpr std::string_view OBJECT_FIELD_KEY{"obj-key"};

            ProjectNode newObject{};
            newObject.addValue(OBJECT_FIELD_KEY.data(), tests::createGenericValue<TestType>());

            projectToWrite.addObject(FIELD_KEY.data(), std::move(newObject));

            WHEN("The project is serialized") {
                writerUnderTest << projectToWrite;

                THEN("It should have the object entry with the correct key") {
                    const nlohmann::json actualJson = tests::readJsonFromString(outStreamRef.str());

                    CHECK(actualJson.contains(FIELD_KEY));
                }

                THEN("It should have the object with the correct values") {
                    const nlohmann::json actualJson = tests::readJsonFromString(outStreamRef.str());

                    REQUIRE(actualJson.contains(FIELD_KEY));

                    const nlohmann::json& subObjJson = actualJson[FIELD_KEY];
                    REQUIRE(subObjJson.contains(OBJECT_FIELD_KEY));

                    CHECK(subObjJson[OBJECT_FIELD_KEY] == tests::createGenericValue<TestType>());
                }
            }
        }
    }
}

SCENARIO("JsonProjectWriter with complex project structures",
         "[unit][sociable][modules][project-management][JsonProjectWriter][complex-structures]") {
    using namespace gc::project_management;
    using namespace gc::project_management::project_io;

    auto outStream = std::make_unique<std::ostringstream>();
    auto& outStreamRef{*outStream};

    GIVEN("A project with four nested layers") {
        using namespace std::string_literals;
        Project projectToWrite{};

        ProjectNode fourthLayer{};
        fourthLayer.addValue("name"s, "FourthLayer"s);
        fourthLayer.addValue("isLast"s, true);
        fourthLayer.addValue("floatValue"s, 40.0);

        ProjectNode thirdLayer{};
        thirdLayer.addValue("name"s, "ThirdLayer"s);
        thirdLayer.addValue("isLast"s, false);
        thirdLayer.addObject("subObject"s, std::move(fourthLayer));

        ProjectNode secondLayer{};
        secondLayer.addValue("name"s, "SecondLayer"s);
        secondLayer.addValue("isLast"s, false);
        secondLayer.addValueArray("values"s, {1, 2, 3, 4, 5});
        secondLayer.addObject("subObject"s, std::move(thirdLayer));

        projectToWrite.addValue("name"s, "Root"s);
        projectToWrite.addValue("isLast"s, false);
        projectToWrite.addValueArray("bools"s, {true, false, false, true});
        projectToWrite.addObject("subObject"s, std::move(secondLayer));

        WHEN("The project is serialized") {
            JsonProjectWriter writerUnderTest{std::move(outStream)};
            writerUnderTest << projectToWrite;

            // We read the serialized JSON.
            nlohmann::json actualJson{};
            REQUIRE_NOTHROW(actualJson = tests::readJsonFromString(outStreamRef.str()));

            THEN("The root layer should have all the correct entries") {
                REQUIRE(actualJson.contains("name"s));
                REQUIRE(actualJson.contains("isLast"s));
                REQUIRE(actualJson.contains("bools"s));
                REQUIRE(actualJson.contains("subObject"s));

                CHECK(actualJson["name"] == "Root");
                CHECK_FALSE(actualJson["isLast"]);
                CHECK(actualJson["bools"].size() == 4);

                REQUIRE(actualJson["subObject"].is_object());

                AND_THEN("The second layer should have all the correct entries") {
                    const nlohmann::json& secondLayerObj = actualJson["subObject"];

                    REQUIRE(secondLayerObj.contains("name"s));
                    REQUIRE(secondLayerObj.contains("isLast"s));
                    REQUIRE(secondLayerObj.contains("values"s));
                    REQUIRE(secondLayerObj.contains("subObject"s));

                    CHECK(secondLayerObj["name"] == "SecondLayer");
                    CHECK_FALSE(secondLayerObj["isLast"]);
                    CHECK(secondLayerObj["values"].size() == 5);

                    REQUIRE(secondLayerObj["subObject"].is_object());

                    AND_THEN("The third layer should have all the correct entries") {
                        const nlohmann::json& thirdLayerObj = secondLayerObj["subObject"];

                        REQUIRE(thirdLayerObj.contains("name"s));
                        REQUIRE(thirdLayerObj.contains("isLast"s));
                        REQUIRE(thirdLayerObj.contains("subObject"s));

                        CHECK(thirdLayerObj["name"] == "ThirdLayer");
                        CHECK_FALSE(thirdLayerObj["isLast"]);

                        REQUIRE(thirdLayerObj["subObject"].is_object());

                        AND_THEN("The fourth layer should have all the correct entries") {
                            const nlohmann::json& fourthLayerObj = thirdLayerObj["subObject"];

                            REQUIRE(fourthLayerObj.contains("name"s));
                            REQUIRE(fourthLayerObj.contains("isLast"s));
                            REQUIRE(fourthLayerObj.contains("floatValue"s));

                            CHECK(fourthLayerObj["name"] == "FourthLayer");
                            CHECK(fourthLayerObj["isLast"]);
                            CHECK(fourthLayerObj["floatValue"] == 40.0);
                        }
                    }
                }
            }
        }
    }
}
