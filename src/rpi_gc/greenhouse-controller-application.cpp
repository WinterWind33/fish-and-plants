// Copyright (C) 2022 Andrea Ballestrazzi
#include <greenhouse-controller-application.hpp>

// Resources
#include <rpi_gc-config-file.hpp>

// C++ STL
#include <utility>
#include <string_view>
#include <sstream>

namespace rpi_gc {

    GreenhouseControllerApplication::GreenhouseControllerApplication(ostream_ref outputStream, istream_ref inputStream) noexcept :
        m_outputStream{std::move(outputStream)},
        m_inputStream{std::move(inputStream)} {}

    void GreenhouseControllerApplication::run() noexcept {
        // The first thing we do is to print the application header,
        // i.e. the first few lines of the application presentation.
        print_app_header();
    }

    void GreenhouseControllerApplication::print_app_header() {
        constexpr std::string_view APPLICATION_NAME{"Greenhouse Controller"};

        m_outputStream.get() << APPLICATION_NAME << " " << GreenhouseControllerApplication::create_version_string();
    }

    std::string GreenhouseControllerApplication::create_version_string() {
        std::ostringstream versionStream{};
        versionStream << rpi_gc_VERSION_MAJOR << ".";
        versionStream << rpi_gc_VERSION_MINOR << ".";
        versionStream << rpi_gc_VERSION_PATCH;

        return versionStream.str();
    }

} // namespace rpi_gc
