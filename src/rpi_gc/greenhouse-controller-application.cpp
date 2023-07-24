// Copyright (C) 2023 Andrea Ballestrazzi
#include <greenhouse-controller-application.hpp>

// Resources
#include <version/version-numbers.hpp>
#include <user-interface/commands-strings.hpp>

// C++ STL
#include <utility>
#include <vector>
#include <cassert>

namespace rpi_gc {

    GreenhouseControllerApplication::GreenhouseControllerApplication(
        ostream_ref outputStream, istream_ref inputStream, logger_pointer mainLogger, gc_project::ProjectController& projectController) noexcept :
        m_outputStream{outputStream},
        m_inputStream{inputStream},
        m_mainLogger{std::move(mainLogger)},
        m_projectController{projectController} {
        assert(static_cast<bool>(m_mainLogger));
    }


    bool GreenhouseControllerApplication::processInputOptions(const std::vector<std::string>& inputArgs) noexcept {
        // We execute the parsing only if there are tokens to parse apart
        // from the application itself (the first token).
        if(inputArgs.size() < 2) {
            return true;
        }

        assert(m_applicationCommand != nullptr);
        m_bCanApplicationCommandExecute = m_applicationCommand->processInputOptions(inputArgs);

        // If we are in this conditional branch then for now we don't have
        // situations where this can be false.
        assert(m_bCanApplicationCommandExecute);

        return true;
    }

    void GreenhouseControllerApplication::run() noexcept {
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
            print_project_path();

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
            if(commandName == strings::commands::EXIT) {
                m_mainLogger->logInfo("EXIT COMMAND ISSUED.");
                continue;
            }

            if(!m_commands.contains(commandName)) {
                // The user typed an unknown command.
                m_outputStream.get() << commandName << ": " << strings::commands::feedbacks::UNRECOGNIZED_COMMAND << " "
                    << strings::commands::feedbacks::TYPE_HELP << std::endl << std::endl;
                continue;
            }

            try {
                if(m_commands.at(commandName)->processInputOptions(lineTokens)) {
                    m_commands[commandName]->execute();
                }
            } catch(const popl::invalid_option& ioexc) {
                m_outputStream.get() << "[ERROR] => Invalid option: " << ioexc.what() << std::endl;
            }


            // We add a new line after the command execution so the user feedback
            // is more clean.
            m_outputStream.get() << std::endl;
        }

        m_outputStream.get() << strings::commands::feedbacks::TEARING_DOWN << std::endl;
        m_mainLogger->logInfo(StringType{strings::commands::feedbacks::TEARING_DOWN});
        teardown();
        m_outputStream.get() << strings::commands::feedbacks::GOODBYE << std::endl;
    }

    void GreenhouseControllerApplication::addSupportedCommand(std::unique_ptr<TerminalCommandType> command) noexcept {
        const std::string commandName{command->getName()};
        assert(!m_commands.contains(commandName));

        m_commands.emplace(commandName, std::move(command));
    }

    void GreenhouseControllerApplication::print_project_path() const {
        constexpr std::string_view INVALID_PROJECT_TITLE{"no-project"};
        const auto& projectController{m_projectController.get()};

        m_outputStream.get() << "controller@";

        if(projectController.hasProject())
            m_outputStream.get() << projectController.getCurrentProject().getTitle();
        else
            m_outputStream.get() << INVALID_PROJECT_TITLE;

        m_outputStream.get() << "$ ";
    }

    void GreenhouseControllerApplication::print_app_header() noexcept {
        m_outputStream.get() << strings::application::NAME << " " << GreenhouseControllerApplication::create_version_string() << std::endl;
        m_outputStream.get() << strings::application::COPYRIGHT_DISCLAIMER << std::endl;
        m_outputStream.get() << std::endl;
        m_outputStream.get() << "-- " << strings::application::TEAM_NAME << " --" << std::endl;
        m_outputStream.get() << std::endl;
    }

    StringType GreenhouseControllerApplication::create_version_string() noexcept {
        OutputStringStream versionStream{};
        versionStream << version::getApplicationVersion();
        return versionStream.str();
    }

    void GreenhouseControllerApplication::teardown() noexcept {
        m_inputStream.get().clear(std::ios::goodbit);
        m_outputStream.get().clear(std::ios::goodbit);

        // The teardown should terminate all the registered systems
        // so we can release the resources safely.
        for(auto& terminableSystem : m_terminableSystems)
            terminableSystem->requestShutdown();
    }

    void GreenhouseControllerApplication::setApplicationCommand(std::unique_ptr<TerminalCommandType> appCommand) noexcept {
        m_applicationCommand = std::move(appCommand);
    }

    void GreenhouseControllerApplication::addEmergencyStoppableSystem(std::shared_ptr<abort_system::EmergencyStoppableSystem> system) noexcept {
        m_emergencyStoppableSystems.push_back(std::move(system));
    }

    void GreenhouseControllerApplication::addTerminableSystem(std::shared_ptr<abort_system::TerminableSystem> system) noexcept {
        m_terminableSystems.push_back(std::move(system));
    }

} // namespace rpi_gc
