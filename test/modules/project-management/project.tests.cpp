// Copyright (c) 2023 Andrea Ballestrazzi

#include <project-management/project.hpp>

#include <testing-core.hpp>

// C++ STL
#include <cstdint>
#include <string_view>

TEST_CASE("Project fields static tests", "[static][modules][project-management][ProjectFields]") {
    using namespace gc::project_management;

    STATIC_CHECK(ProjectFieldValue<bool>);
    STATIC_CHECK(ProjectFieldValue<const bool&>);
    STATIC_CHECK(ProjectFieldValue<std::int32_t>);
    STATIC_CHECK(ProjectFieldValue<const std::int32_t>);
    STATIC_CHECK(ProjectFieldValue<const std::int32_t&>);
    STATIC_CHECK(ProjectFieldValue<std::int64_t>);
    STATIC_CHECK(ProjectFieldValue<const std::int64_t&>);
    STATIC_CHECK(ProjectFieldValue<std::size_t>);
    STATIC_CHECK(ProjectFieldValue<const std::size_t&>);
    STATIC_CHECK(ProjectFieldValue<float>);
    STATIC_CHECK(ProjectFieldValue<const float&>);
    STATIC_CHECK(ProjectFieldValue<double>);
    STATIC_CHECK(ProjectFieldValue<const double&>);
    STATIC_CHECK(ProjectFieldValue<std::string>);
    STATIC_CHECK(ProjectFieldValue<const std::string&>);

    STATIC_CHECK(ProjectFieldKey<std::string>);
    STATIC_CHECK(ProjectFieldKey<const char*>);
}

TEMPLATE_TEST_CASE("ProjectNode unit tests",
                   "[unit][solitary][modules][project-management][ProjectNode]", std::int64_t,
                   double, std::string) {
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
            nodeUnderTest.addValueArray(std::string{KEY_VAL},
                                        std::vector<TestType>{TestType{0}, TestType{0}});

            THEN("The node should not have one single values") {
                const auto& values{nodeUnderTest.getValues()};

                CHECK(values.empty());
            }

            THEN("The node should have one value array") {
                const auto& valueArrays{nodeUnderTest.getValuesArrays()};

                REQUIRE(valueArrays.contains(std::string{KEY_VAL}));
                CHECK(valueArrays.at(std::string{KEY_VAL}).size() == 2);
            }
        }

        WHEN("A new object is added") {
            constexpr std::string_view KEY_VAL{"key-value"};

            ProjectFieldObject newObject{};
            newObject.addValue("int_key", 0)
                .addValue("double_key", 0.0)
                .addValueArray("arr_key", {0.0, 0.0, 0.0});

            nodeUnderTest.addObject(std::string{KEY_VAL}, std::move(newObject));

            THEN("There should be one object node") {
                const auto& objects{nodeUnderTest.getObjects()};

                REQUIRE(objects.contains(std::string{KEY_VAL}));
            }
        }
    }
}

TEMPLATE_TEST_CASE("ProjectNode queries unit tests",
                   "[unit][solitary][modules][project-management][ProjectNode][queries]", bool,
                   char, std::int16_t, std::int32_t, std::int64_t, std::uint16_t, std::uint32_t,
                   std::uint64_t, float, double, std::string) {
    using namespace gc::project_management;

    ProjectNode nodeUnderTest{};

    constexpr std::string_view KEY_VAL{"key-value"};

    if constexpr (std::is_floating_point_v<TestType>) {
        nodeUnderTest.addValue(KEY_VAL.data(), 0.0);
    } else if constexpr (std::is_same_v<TestType, bool>) {
        nodeUnderTest.addValue(KEY_VAL.data(), false);
    } else if constexpr (std::is_integral_v<TestType> && std::is_signed_v<TestType>) {
        nodeUnderTest.addValue(KEY_VAL.data(), -42);
    } else if constexpr (std::is_integral_v<TestType> && std::is_unsigned_v<TestType>) {
        nodeUnderTest.addValue(KEY_VAL.data(), 42ull);
    } else if constexpr (std::is_same_v<TestType, std::string>) {
        nodeUnderTest.addValue(KEY_VAL.data(), std::string{"test"});
    } else {
        static_assert(details::InvalidVariantType<TestType>);
    }

    WHEN("A value is queried") {
        THEN("It shouldn't throw any exception") {
            CHECK_NOTHROW(nodeUnderTest.getValue<TestType>(KEY_VAL.data()));
        }
    }
}
