// Copyright (C) 2022 Andrea Ballestrazzi
#include <greenhouse-controller-application.hpp>

// Resources
#include <rpi_gc-config-file.hpp>
#include <user-interface/commands-strings.hpp>

// C++ STL
#include <utility>
#include <vector>

namespace rpi_gc {

    GreenhouseControllerApplication::GreenhouseControllerApplication(ostream_ref outputStream, istream_ref inputStream, std::unique_ptr<option_parser> optionParser) noexcept :
        m_outputStream{std::move(outputStream)},
        m_inputStream{std::move(inputStream)} {}

    bool GreenhouseControllerApplication::processInputOptions(const std::int32_t argc, const CharType* const argv[]) noexcept {
        std::vector<StringType> tokens{};
        for(std::int32_t i{}; i < argc; ++i)
            tokens.push_back(argv[i]);

        // We execute the parsing only if there are tokens to parse apart
        // from the application itselt (the first token).
        if(tokens.size() > 1) {
            assert(m_applicationCommand != nullptr);
            m_bCanApplicationCommandExecute = m_applicationCommand->processInputOptions(tokens);

            // If we are in this conditional branch then for now we don't have
            // situations where this can be false.
            assert(m_bCanApplicationCommandExecute);
        }

        return true;
    }

    void GreenhouseControllerApplication::run() noexcept {
        using StringView = std::basic_string_view<CharType>;

        // Firstly we run the the application command if the user
        // typed some options during the application launching.
        if(m_bCanApplicationCommandExecute) {
            assert(m_applicationCommand != nullptr);

            // If the execution is already satisfied the we can safely exit
            // the execution (maybe the user typed --help or similar).
            const bool bCanProceed{m_applicationCommand->execute()};
            if(!bCanProceed) {
                teardown();
                return;
            }
        }

        // The first thing we do is to print the application header,
        // i.e. the first few lines of the application presentation.
        print_app_header();

        // Now we begin the user input loop.
        std::string inputLine{};

        m_outputStream.get() << strings::commands::feedbacks::TYPE_HELP << std::endl;

        while(inputLine != strings::commands::EXIT && m_inputStream.get().good()) {
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
            if(commandName == strings::commands::EXIT)
                continue;

            if(!m_commands.contains(commandName)) {
                // The user typed an unknown command.
                m_outputStream.get() << commandName << ": " << strings::commands::feedbacks::UNRECOGNIZED_COMMAND << " "
                    << strings::commands::feedbacks::TYPE_HELP << std::endl << std::endl;
                continue;
            }

            const bool bCanExec = m_commands.at(commandName)->processInputOptions(lineTokens);
            if(bCanExec) {
                m_commands[commandName]->execute();
            }

            // We add a new line after the command execution so the user feedback
            // is more clean.
            m_outputStream.get() << std::endl;
        }

        m_outputStream.get() << strings::commands::feedbacks::TEARING_DOWN;
        teardown();
        m_outputStream.get() << "Done." << std::endl;
        m_outputStream.get() << strings::commands::feedbacks::GOODBYE << std::endl;
    }

    void GreenhouseControllerApplication::addSupportedCommand(std::unique_ptr<TerminalCommandType> command, std::unique_ptr<option_parser> commandOptionParser) noexcept {
        const StringType commandName{command->getName()};
        assert(!m_commands.contains(commandName));

        m_commands.emplace(commandName, std::move(command));
    }

    void GreenhouseControllerApplication::print_app_header() noexcept {
        using StringView = std::basic_string_view<CharType>;

        m_outputStream.get() << strings::application::NAME << " " << GreenhouseControllerApplication::create_version_string() << std::endl;
        m_outputStream.get() << strings::application::COPYRIGHT_DISCLAIMER << std::endl;
        m_outputStream.get() << std::endl;
        m_outputStream.get() << "-- " << strings::application::TEAM_NAME << " --" << std::endl;
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
    }

    void GreenhouseControllerApplication::setApplicationCommand(std::unique_ptr<TerminalCommandType> appCommand) noexcept {
        m_applicationCommand = std::move(appCommand);
    }

} // namespace rpi_gc
