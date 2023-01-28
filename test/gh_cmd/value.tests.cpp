// Copyright (c) 2022 Andrea Ballestrazzi
#include <gh_cmd/gh_cmd.hpp>

// Test doubles
#include <gh_cmd/test-doubles/option-visitor.mock.hpp>

// Testing core
#include <testing-core.hpp>

// C++ STL
#include <cstdint>
#include <chrono>
#include <string>
#include <string_view>

TEMPLATE_TEST_CASE("Value unit tests", "[unit][sociable][gh_cmd][Value]",
    std::int32_t, float, double, std::string) {

    using namespace gh_cmd;

    SECTION("Getters") {
        constexpr char EXPECTED_SHORT_NAME{'h'};
        constexpr std::string_view EXPECTED_LONG_NAME{"help"};
        constexpr std::string_view EXPECTED_DESCRIPTION{"Produces help message"};

        Value<char, TestType> valueUnderTest{EXPECTED_SHORT_NAME, std::string{EXPECTED_LONG_NAME}, std::string{EXPECTED_DESCRIPTION}};

        SECTION("It should retrieve the correct short name") {
            CHECK(valueUnderTest.getShortName() == EXPECTED_SHORT_NAME);
        }

        SECTION("It should retrieve the correct long name") {
            CHECK(valueUnderTest.getLongName() == EXPECTED_LONG_NAME);
        }

        SECTION("It should retrieve the correct description") {
            CHECK(valueUnderTest.getDescription() == EXPECTED_DESCRIPTION);
        }
    }

    SECTION("Visiting the implementation") {
        constexpr char DUMMY_SHORT_NAME{'h'};
        constexpr std::string_view DUMMY_LONG_NAME{"help"};
        constexpr std::string_view DUMMY_DESCRIPTION{"Produces help message"};

        Value<char, TestType> valueUnderTest{DUMMY_SHORT_NAME, std::string{DUMMY_LONG_NAME}, std::string{DUMMY_DESCRIPTION}};

        SECTION("Method: acceptVisitor() non-const") {
            using BaseOptionImplType = CommandOption<char>::base_impl_type;
            using VisitorArgumentType = std::shared_ptr<BaseOptionImplType>;
            using OptionVisitorMockType = mocks::OptionVisitorMock<VisitorArgumentType>;

            SECTION("Should call the visitor visit() method") {
                OptionVisitorMockType optionVisitorMock{};
                EXPECT_CALL(optionVisitorMock, visit).Times(1);

                valueUnderTest.acceptVisitor(optionVisitorMock);
            }

            SECTION("Should pass to the visit() method a valid pointer") {
                OptionVisitorMockType optionVisitorMock{};
                EXPECT_CALL(optionVisitorMock, visit).WillOnce([](VisitorArgumentType option){
                    CHECK(option != nullptr);
                });

                valueUnderTest.acceptVisitor(optionVisitorMock);
            }
        }

        SECTION("Method: acceptVisitor() const") {
            using BaseOptionImplType = CommandOption<char>::base_impl_type;
            using VisitorArgumentType = std::shared_ptr<const BaseOptionImplType>;
            using OptionVisitorMockType = testing::StrictMock<mocks::ConstOptionVisitorMock<VisitorArgumentType>>;

            SECTION("Should call the visitor visit() method") {
                OptionVisitorMockType optionVisitorMock{};
                EXPECT_CALL(optionVisitorMock, visit).Times(1);

                valueUnderTest.acceptVisitor(optionVisitorMock);
            }

            SECTION("Should pass to the visit() method a valid pointer") {
                OptionVisitorMockType optionVisitorMock{};
                EXPECT_CALL(optionVisitorMock, visit).WillOnce([](VisitorArgumentType option){
                    CHECK(option != nullptr);
                });

                valueUnderTest.acceptVisitor(optionVisitorMock);
            }
        }

        GIVEN("A Value object") {
            constexpr char DUMMY_SHORT_NAME{'h'};
            constexpr std::string_view DUMMY_LONG_NAME{"help"};
            constexpr std::string_view DUMMY_DESCRIPTION{"Produces help message"};

            Value<char, TestType> valueUnderTest{DUMMY_SHORT_NAME, std::string{DUMMY_LONG_NAME}, std::string{DUMMY_DESCRIPTION}};
            WHEN("Value::clear() is called") {
                valueUnderTest.clear();

                THEN("The value should not be set") {
                    CHECK_FALSE(valueUnderTest.isSet());
                }
            }
        }
    }
}
