// Copyright (C) 2022 Andrea Ballestrazzi
#include <greenhouse-controller-application.hpp>

#include <gh_cmd/gh_cmd.hpp>

// C++ STL
#include <iostream>

// This is the entry point of the application. Here, it starts
// the main execution of the greenhouse controller.
int main(int argc, char* argv[]) {
    using namespace rpi_gc;

    // We pass the standard output as the output stream and the standard input as the input stream for now.
    // This may change in the future.
    GreenhouseControllerApplication mainApplication{std::cout, std::cin, std::make_unique<gh_cmd::DefaultOptionParser<CharType>>("Terminal option parser")};
    mainApplication.run();

    return 0;
}
