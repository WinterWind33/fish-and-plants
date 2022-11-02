// Copyright (C) 2022 Andrea Ballestrazzi
#include <greenhouse-controller-application.hpp>

// Resources
#include <rpi_gc-config-file.hpp>
#include <gh_cmd/gh_cmd.hpp>

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
        using StringView = std::basic_string_view<CharType>;

        constexpr StringView APPLICATION_NAME{"Greenhouse Controller"};
        constexpr StringView COPYRIGHT_DISCLAIMER{"Copyright (c) 2022 Andrea Ballestrazzi"};
        constexpr StringView TEAM_CREDIT{"-- Fish&Plants Team --"};

        m_outputStream.get() << APPLICATION_NAME << " " << GreenhouseControllerApplication::create_version_string() << std::endl;
        m_outputStream.get() << COPYRIGHT_DISCLAIMER << std::endl;
        m_outputStream.get() << std::endl;
        m_outputStream.get() << TEAM_CREDIT << std::endl;
        m_outputStream.get() << std::endl;

        // Now we begin the user input loop.
        constexpr StringView EXIT_COMMAND{"exit"};
        std::string inputLine{};

        while(inputLine != EXIT_COMMAND) {
            m_outputStream.get() << "user@controller/home$ ";
            std::getline(m_inputStream.get(), inputLine);
        }

        m_outputStream.get() << "Goodbye." << std::endl;
    }

    StringType GreenhouseControllerApplication::create_version_string() {
        OutputStringStream versionStream{};
        versionStream << rpi_gc_VERSION_MAJOR << ".";
        versionStream << rpi_gc_VERSION_MINOR << ".";
        versionStream << rpi_gc_VERSION_PATCH;

        return versionStream.str();
    }

} // namespace rpi_gc
