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
        using StringView = std::basic_string_view<CharType>;

        // The first thing we do is to print the application header,
        // i.e. the first few lines of the application presentation.
        print_app_header();

        // Now we begin the user input loop.
        constexpr StringView EXIT_COMMAND{"exit"};
        std::string inputLine{};

        while(inputLine != EXIT_COMMAND && m_inputStream.get().good()) {
            m_outputStream.get() << "user@controller/home$ ";
            std::getline(m_inputStream.get(), inputLine);

            // Empty line: we can skip this iteration as the user hasn't typed
            // anything.
            if(inputLine.empty())
                continue;

            InputStringStream inputLineStream{inputLine};
            std::vector<StringType> lineTokens{};
            for(StringType currentToken{}; inputLineStream >> currentToken;)
                lineTokens.push_back(std::move(currentToken));

            assert(lineTokens.size() > 0);
            const StringType commandName{lineTokens[0]};

            // If the user requested to exit the program we can exit the
            // execution.
            if(commandName == EXIT_COMMAND)
                continue;

            if(!m_commandsOptionParsers.contains(commandName)) {
                constexpr StringView UNKNOWN_COMMAND_FEEDBACK{"command not recognized."};

                // The user typed an unknown command.
                m_outputStream.get() << commandName << ": " << UNKNOWN_COMMAND_FEEDBACK << std::endl << std::endl;
                continue;
            }

            assert(m_commandsOptionParsers[commandName] != nullptr);
            m_commandsOptionParsers[commandName]->parse(lineTokens);
        }

        m_outputStream.get() << "Tearing down...";
        teardown();
        m_outputStream.get() << "Done." << std::endl;
        m_outputStream.get() << "Goodbye." << std::endl;
    }

    void GreenhouseControllerApplication::addSupportedCommand(StringType commandName, std::unique_ptr<gh_cmd::OptionParser<CharType>> commandOptionParser) noexcept {
        assert(!m_commandsOptionParsers.contains(commandName));

        m_commandsOptionParsers.emplace(std::move(commandName), std::move(commandOptionParser));
    }

    void GreenhouseControllerApplication::print_app_header() noexcept {
        using StringView = std::basic_string_view<CharType>;

        constexpr StringView APPLICATION_NAME{"Greenhouse Controller"};
        constexpr StringView COPYRIGHT_DISCLAIMER{"Copyright (c) 2022 Andrea Ballestrazzi"};
        constexpr StringView TEAM_CREDIT{"-- Fish&Plants Team --"};

        m_outputStream.get() << APPLICATION_NAME << " " << GreenhouseControllerApplication::create_version_string() << std::endl;
        m_outputStream.get() << COPYRIGHT_DISCLAIMER << std::endl;
        m_outputStream.get() << std::endl;
        m_outputStream.get() << TEAM_CREDIT << std::endl;
        m_outputStream.get() << std::endl;
    }

    StringType GreenhouseControllerApplication::create_version_string() noexcept {
        OutputStringStream versionStream{};
        versionStream << rpi_gc_VERSION_MAJOR << ".";
        versionStream << rpi_gc_VERSION_MINOR << ".";
        versionStream << rpi_gc_VERSION_PATCH;

        return versionStream.str();
    }

    void GreenhouseControllerApplication::teardown() noexcept {
        m_inputStream.get().clear(std::ios::goodbit);
        m_outputStream.get().clear(std::ios::goodbit);

        m_commandsOptionParsers.clear();
    }

} // namespace rpi_gc
