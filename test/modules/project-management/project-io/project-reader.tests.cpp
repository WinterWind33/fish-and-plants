// Copyright (c) 2023 Andrea Ballestrazzi

#include <project-management/project-io/project-reader.hpp>

#include <testing-core.hpp>

// C++ STL
#include <stdexcept>

TEST_CASE("ProjectReader Factory tests", "[integration][modules][project-management][project-io][ProjectReaderFactories]") {
    using namespace gc::project_management;

    SECTION("CreateJsonProjectFileReader() Function") {
        SECTION("Should throw a std::invalid_argument exception if the input file does not exist") {
            CHECK_THROWS_AS(project_io::CreateJsonProjectFileReader("inexistent_dir/inexistent_file.json"), std::invalid_argument);
        }
    }
}
