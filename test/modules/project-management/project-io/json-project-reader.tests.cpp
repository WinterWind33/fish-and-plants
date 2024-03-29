// Copyright (c) 2023 Andrea ballestrazzi

#include <project-management/project.hpp>
#include <src/project-io/json-project-reader.hpp>

#include <testing-core.hpp>

// C++ STL
#include <chrono>
#include <sstream>

namespace tests {

template <gc::project_management::ProjectFieldValue ValueType>
ValueType createTestValue() {
    if constexpr (std::is_same_v<ValueType, bool>)
        return true;
    if constexpr (std::is_integral_v<ValueType> && !std::is_same_v<ValueType, bool>) {
        if constexpr (std::is_unsigned_v<ValueType>)
            return 42u;
        else
            return -42;
    }
    if constexpr (std::is_floating_point_v<ValueType>)
        return 42.0;
    if constexpr (std::is_same_v<ValueType, std::string>)
        return std::string{"test-str"};

    return ValueType{};
}

} // namespace tests

TEST_CASE("JsonProjectReader unit tests",
          "[unit][sociable][project-management][project-io][JsonProjectReader]") {
    using namespace gc::project_management;

    GIVEN("A project with invalid version") {
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

        const Project expectedProject{
            std::chrono::system_clock::from_time_t(1672576240), "test-title",
            semver::version{1, 2, 3}
        };

        project_io::JsonProjectReader projectReaderUnderTest{std::move(inputStream)};

        WHEN("The project is read") {
            Project inputProject{};
            projectReaderUnderTest >> inputProject;

            THEN("The read project should be correct") {
                CHECK(SoftCompareProjects(inputProject, expectedProject));
            }
        }
    }
}

TEMPLATE_TEST_CASE("JsonProjectReader values unit tests",
                   "[unit][sociable][modules][project-management][JsonProjectReader][values]", bool,
                   std::int64_t, std::uint64_t, double, std::string) {
    static_assert(gc::project_management::ProjectFieldValue<TestType>);

    using namespace gc::project_management;
    using namespace gc::project_management::project_io;

    GIVEN("A project with a value") {
        std::ostringstream jsonStream{};
        // Here we generate the JSON string based on TestType's type.
        jsonStream << R"(
            {
                "creation_timedate": 1672576240,
                "title": "test-title",
                "version": "1.2.3",
                "test-value": )";
        const TestType testValue{tests::createTestValue<TestType>()};
        if constexpr (std::is_same_v<TestType, bool>)
            jsonStream << std::boolalpha << testValue;
        else if constexpr (std::is_same_v<TestType, double>)
            jsonStream << std::fixed << testValue;
        else if constexpr (std::is_same_v<TestType, std::string>)
            jsonStream << '\"' << testValue << '\"';
        else
            jsonStream << testValue;

        jsonStream << R"(
            }
        )";

        auto inputStream{std::make_unique<std::istringstream>(jsonStream.str())};

        Project expectedProject{
            std::chrono::system_clock::from_time_t(1672576240), "test-title",
            semver::version{1, 2, 3}
        };
        expectedProject.addValue("test-value", 42);

        WHEN("The project is read") {
            project_io::JsonProjectReader projectReaderUnderTest{std::move(inputStream)};
            Project inputProject{};
            projectReaderUnderTest >> inputProject;

            THEN("The read project basic data should be correct") {
                REQUIRE(SoftCompareProjects(inputProject, expectedProject));

                AND_THEN("The read project should have the correct value") {
                    REQUIRE(inputProject.containsValue("test-value"));
                    CHECK(inputProject.getValue<TestType>("test-value") == testValue);
                }
            }
        }
    }

    GIVEN("A project with a value array") {
        std::ostringstream jsonStream{};
        // Here we generate the JSON string based on TestType's type.
        jsonStream << R"(
            {
                "creation_timedate": 1672576240,
                "title": "test-title",
                "version": "1.2.3",
                "test-value-array": [)";
        const TestType testValue{tests::createTestValue<TestType>()};

        constexpr std::size_t ARRAY_SIZE{5};
        for (std::size_t i{0}; i < ARRAY_SIZE; ++i) {
            if (i != 0)
                jsonStream << ", ";

            if constexpr (std::is_same_v<TestType, bool>)
                jsonStream << std::boolalpha << testValue;
            else if constexpr (std::is_same_v<TestType, double>)
                jsonStream << std::fixed << testValue;
            else if constexpr (std::is_same_v<TestType, std::string>)
                jsonStream << '\"' << testValue << '\"';
            else
                jsonStream << testValue;
        }

        jsonStream << R"(
            ]
        }
        )";

        auto inputStream{std::make_unique<std::istringstream>(jsonStream.str())};

        Project expectedProject{
            std::chrono::system_clock::from_time_t(1672576240), "test-title",
            semver::version{1, 2, 3}
        };
        expectedProject.addValueArray("test-value-array", {testValue});

        WHEN("The project is read") {
            project_io::JsonProjectReader projectReaderUnderTest{std::move(inputStream)};
            Project inputProject{};
            projectReaderUnderTest >> inputProject;

            THEN("The read project basic data should be correct") {
                REQUIRE(SoftCompareProjects(inputProject, expectedProject));

                AND_THEN("The read project should have the correct value array") {
                    REQUIRE(inputProject.containsValueArray("test-value-array"));
                    const auto& arr{inputProject.getValueArray("test-value-array")};

                    for (const auto& val : arr)
                        CHECK(std::get<TestType>(val) == testValue);
                }
            }
        }
    }
}

TEST_CASE("JsonProjectReader objects unit tests",
          "[unit][sociable][modules][project-management][JsonProjectReader][objects]") {
    using namespace gc::project_management;
    using namespace gc::project_management::project_io;

    GIVEN("A project with an object that has a value") {
        std::ostringstream jsonStream{};
        // Here we generate the JSON string based on TestType's type.
        jsonStream << R"(
            {
                "creation_timedate": 1672576240,
                "title": "test-title",
                "version": "1.2.3",
                "test-value-object": {
                    "test-value": 42
                }
            }
        )";

        auto inputStream{std::make_unique<std::istringstream>(jsonStream.str())};

        Project expectedProject{
            std::chrono::system_clock::from_time_t(1672576240), "test-title",
            semver::version{1, 2, 3}
        };

        WHEN("The project is read") {
            project_io::JsonProjectReader projectReaderUnderTest{std::move(inputStream)};
            Project inputProject{};
            projectReaderUnderTest >> inputProject;

            THEN("The read project basic data should be correct") {
                REQUIRE(SoftCompareProjects(inputProject, expectedProject));

                AND_THEN("The read project should have the correct value object") {
                    REQUIRE(inputProject.containsObject("test-value-object"));
                    const auto& obj{inputProject.getObject("test-value-object")};

                    REQUIRE(obj.containsValue("test-value"));
                    CHECK(obj.getValue<std::uint64_t>("test-value") == 42);
                }
            }
        }
    }
}

TEST_CASE("JsonProjectReader complex project structure unit tests",
          "[unit][sociable][modules][project-management][JsonProjectReader][objects]") {
    using namespace gc::project_management;
    using namespace gc::project_management::project_io;

    GIVEN("A project with all the value types in it") {
        std::ostringstream jsonStream{};
        jsonStream << R"(
            {
                "creation_timedate": 1672576240,
                "title": "test-title",
                "version": "1.2.3",
                "test-value": 42,
                "test-value-array": [1, 2, 3],
                "test-value-object": {
                    "test-value": 42
                }
            }
        )";

        auto inputStream{std::make_unique<std::istringstream>(jsonStream.str())};

        Project expectedProject{
            std::chrono::system_clock::from_time_t(1672576240), "test-title",
            semver::version{1, 2, 3}
        };
        expectedProject.addValue("test-value", 42);
        expectedProject.addValueArray("test-value-array", {1, 2, 3});

        ProjectNode obj{};
        obj.addValue("test-value", 42);
        expectedProject.addObject("test-value-object", std::move(obj));

        WHEN("The project is read") {
            project_io::JsonProjectReader projectReaderUnderTest{std::move(inputStream)};
            Project inputProject{};
            projectReaderUnderTest >> inputProject;

            THEN("The read project basic data should be correct") {
                REQUIRE(SoftCompareProjects(inputProject, expectedProject));

                AND_THEN("The read project should have the correct value") {
                    REQUIRE(inputProject.containsValue("test-value"));
                    CHECK(inputProject.getValue<std::uint64_t>("test-value") == 42);
                }

                AND_THEN("The read project should have the correct value object") {
                    REQUIRE(inputProject.containsObject("test-value-object"));
                    const auto& obj{inputProject.getObject("test-value-object")};

                    REQUIRE(obj.containsValue("test-value"));
                    CHECK(obj.getValue<std::uint64_t>("test-value") == 42);
                }

                AND_THEN("The read project should have the correct value array") {
                    REQUIRE(inputProject.containsValueArray("test-value-array"));
                    const auto& arr{inputProject.getValueArray("test-value-array")};

                    CHECK(arr.size() == 3);
                    CHECK(std::get<std::uint64_t>(arr[0]) == 1);
                    CHECK(std::get<std::uint64_t>(arr[1]) == 2);
                    CHECK(std::get<std::uint64_t>(arr[2]) == 3);
                }
            }
        }
    }

    GIVEN("A project with four layers") {
        std::ostringstream jsonStream{};
        jsonStream << R"(
            {
                "creation_timedate": 1672576240,
                "title": "test-title",
                "version": "1.2.3",
                "test-value": 42,
                "test-value-array": [1, 2, 3],
                "test-value-object": {
                    "test-value": true,
                    "test-value-array": [true, false, true],
                    "test-value-object": {
                        "test-value": 42.0,
                        "test-value-array": [-54.0, -89.2, 1236.89],
                        "test-value-object": {
                            "test-value": "test-str",
                            "test-value-array": ["test-str0", "test-str1", "test-str2" ]
                        }
                    }
                }
            }
        )";

        auto inputStream{std::make_unique<std::istringstream>(jsonStream.str())};

        Project expectedProject{
            std::chrono::system_clock::from_time_t(1672576240), "test-title",
            semver::version{1, 2, 3}
        };
        expectedProject.addValue("test-value", 42);
        expectedProject.addValueArray("test-value-array", {1, 2, 3});

        ProjectNode obj{};
        obj.addValue("test-value", 42);
        obj.addValueArray("test-value-array", {1, 2, 3});

        ProjectNode obj2{};
        obj2.addValue("test-value", 42);
        obj2.addValueArray("test-value-array", {1, 2, 3});

        ProjectNode obj3{};
        obj3.addValue("test-value", 42);
        obj3.addValueArray("test-value-array", {1, 2, 3});

        obj2.addObject("test-value-object", std::move(obj3));
        obj.addObject("test-value-object", std::move(obj2));
        expectedProject.addObject("test-value-object", std::move(obj));

        WHEN("The project is read") {
            project_io::JsonProjectReader projectReaderUnderTest{std::move(inputStream)};
            Project inputProject{};
            projectReaderUnderTest >> inputProject;

            THEN("The read project basic data should be correct") {
                REQUIRE(SoftCompareProjects(inputProject, expectedProject));

                AND_THEN("The first layer should be correct") {
                    REQUIRE(inputProject.containsValue("test-value"));
                    CHECK(inputProject.getValue<std::uint64_t>("test-value") == 42);

                    REQUIRE(inputProject.containsValueArray("test-value-array"));
                    const auto& arr{inputProject.getValueArray("test-value-array")};

                    CHECK(arr.size() == 3);
                    CHECK(std::get<std::uint64_t>(arr[0]) == 1);
                    CHECK(std::get<std::uint64_t>(arr[1]) == 2);
                    CHECK(std::get<std::uint64_t>(arr[2]) == 3);

                    REQUIRE(inputProject.containsObject("test-value-object"));
                    const auto& inObj{inputProject.getObject("test-value-object")};

                    AND_THEN("The second layer should be correct") {
                        REQUIRE(inObj.containsValue("test-value"));
                        CHECK(inObj.getValue<bool>("test-value"));

                        REQUIRE(inObj.containsValueArray("test-value-array"));
                        const auto& arr2{inObj.getValueArray("test-value-array")};

                        CHECK(arr2.size() == 3);
                        CHECK(std::get<bool>(arr2[0]));
                        CHECK_FALSE(std::get<bool>(arr2[1]));
                        CHECK(std::get<bool>(arr2[2]));

                        REQUIRE(inObj.containsObject("test-value-object"));
                        const auto& inObj2{inObj.getObject("test-value-object")};

                        AND_THEN("The third layer should be correct") {
                            REQUIRE(inObj2.containsValue("test-value"));
                            CHECK(inObj2.getValue<double>("test-value") == 42.0);

                            REQUIRE(inObj2.containsValueArray("test-value-array"));
                            const auto& arr3{inObj2.getValueArray("test-value-array")};

                            CHECK(arr3.size() == 3);
                            CHECK(std::get<double>(arr3[0]) == -54.0);
                            CHECK(std::get<double>(arr3[1]) == -89.2);
                            CHECK(std::get<double>(arr3[2]) == 1236.89);

                            REQUIRE(inObj2.containsObject("test-value-object"));
                            const auto& inObj3{inObj2.getObject("test-value-object")};

                            AND_THEN("The fourth layer should be correct") {
                                REQUIRE(inObj3.containsValue("test-value"));
                                CHECK(inObj3.getValue<std::string>("test-value") == "test-str");

                                REQUIRE(inObj3.containsValueArray("test-value-array"));
                                const auto& arr4{inObj3.getValueArray("test-value-array")};

                                CHECK(std::get<std::string>(arr4[0]) == "test-str0");
                                CHECK(std::get<std::string>(arr4[1]) == "test-str1");
                                CHECK(std::get<std::string>(arr4[2]) == "test-str2");
                            }
                        }
                    }
                }
            }
        }
    }
}
