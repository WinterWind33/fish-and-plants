// Copyright (C) 2022 Andrea Ballestrazzi
#include <greenhouse-controller-application.hpp>

// C++ STL
#include <iostream>
#include <memory>

// This is the entry point of the application. Here, it starts
// the main execution of the greenhouse controller.
int main(int argc, char* argv[]) {
    using namespace rpi_gc;

    // We pass the standard output as the output stream and the standard input as the input stream for now.
    // This may change in the future.
    std::unique_ptr<Application> mainApplication{std::make_unique<GreenhouseControllerApplication>(std::cout, std::cin)};
    mainApplication->run();

    return 0;
}
