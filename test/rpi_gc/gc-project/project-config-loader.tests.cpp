// Copyright (c) 2023 Andrea Ballestrazzi
#include <gc-project/project-config-loader.hpp>

// Test Doubles
#include <gh_log/test-doubles/logger.mock.hpp>

#include <testing-core.hpp>

TEST_CASE("ProjectConfigLoader unit tests",
          "[unit][sociable][rpi_gc][gc-project][ProjectConfigLoader]") {
    using namespace rpi_gc::gc_project;

    // Stub logger
    ::testing::NiceMock<gh_log::mocks::LoggerMock> stubLogger{};

    ProjectConfigLoader projectConfigLoaderUnderTest{stubLogger};
}
