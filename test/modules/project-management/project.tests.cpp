// Copyright (c) 2023 Andrea Ballestrazzi

#include <project-management/project.hpp>

#include <testing-core.hpp>

// C++ STL
#include <cstdint>
#include <string_view>

TEST_CASE("Project fields static tests", "[static][modules][project-management][ProjectFields]") {
    using namespace gc::project_management;

    STATIC_CHECK(ProjectFieldValue<bool>);
    STATIC_CHECK(ProjectFieldValue<std::int32_t>);
    STATIC_CHECK(ProjectFieldValue<std::int64_t>);
    STATIC_CHECK(ProjectFieldValue<std::size_t>);
    STATIC_CHECK(ProjectFieldValue<float>);
    STATIC_CHECK(ProjectFieldValue<double>);
    STATIC_CHECK(ProjectFieldValue<std::string>);

    STATIC_CHECK(ProjectFieldKey<std::string>);
    STATIC_CHECK(ProjectFieldKey<const char*>);
}

TEMPLATE_TEST_CASE("ProjectNode unit tests", "[unit][solitary][modules][project-management][ProjectNode]",
    std::int32_t, double, std::string) {
    using namespace gc::project_management;

    GIVEN("An empty ProjectNode object") {
        ProjectNode nodeUnderTest{};

        WHEN("A new value is added") {
            constexpr std::string_view KEY_VAL{"key-value"};
            nodeUnderTest.addValue(std::string{KEY_VAL}, TestType{});

            THEN("The node should have one value") {
                const auto& values{nodeUnderTest.getValues()};

                REQUIRE(values.size() == 1);
                REQUIRE(values.contains(std::string{KEY_VAL}));
                CHECK(std::get<TestType>(values.at(std::string{KEY_VAL})) == TestType{});
            }
        }

        WHEN("A new value array is added") {
            constexpr std::string_view KEY_VAL("key-value");
            nodeUnderTest.addValueArray(std::string{KEY_VAL}, std::vector<TestType>{
                TestType{0},
                TestType{0}
            });


        }
    }
}
