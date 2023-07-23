// Copyright (c) 2023 Andrea ballestrazzi

#include <src/project-io/json-project-reader.hpp>
#include <project-management/project.hpp>

#include <testing-core.hpp>

// C++ STL
#include <sstream>
#include <chrono>

namespace tests {

    template<gc::project_management::ProjectFieldValue ValueType>
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

TEST_CASE("JsonProjectReader unit tests", "[unit][sociable][project-management][project-io][JsonProjectReader]") {
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

        const Project expectedProject{std::chrono::system_clock::from_time_t(1672576240), "test-title", semver::version{1, 2, 3}};

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

TEMPLATE_TEST_CASE("JsonProjectReader values unit tests", "[unit][sociable][modules][project-management][JsonProjectReader][values]",
    bool, std::int64_t, std::uint64_t, double, std::string) {
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

        Project expectedProject{std::chrono::system_clock::from_time_t(1672576240), "test-title", semver::version{1, 2, 3}};
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
        for(std::size_t i{0}; i < ARRAY_SIZE; ++i) {
            if(i != 0)
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

        Project expectedProject{std::chrono::system_clock::from_time_t(1672576240), "test-title", semver::version{1, 2, 3}};
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

                    for(const auto& val : arr)
                        CHECK(std::get<TestType>(val) == testValue);
                }
            }
        }
    }
}

TEST_CASE("JsonProjectReader objects unit tests", "[unit][sociable][modules][project-management][JsonProjectReader][objects]") {
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

        Project expectedProject{std::chrono::system_clock::from_time_t(1672576240), "test-title", semver::version{1, 2, 3}};

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

TEST_CASE("JsonProjectReader complex project structure unit tests", "[unit][sociable][modules][project-management][JsonProjectReader][objects]") {
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

        Project expectedProject{std::chrono::system_clock::from_time_t(1672576240), "test-title", semver::version{1, 2, 3}};
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
}
