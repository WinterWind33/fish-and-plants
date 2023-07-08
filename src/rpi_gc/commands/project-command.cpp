// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands/project-command.hpp>

// C++ STL
#include <cassert>
#include <iostream> // TODO: Remove this
#include <algorithm>

namespace rpi_gc::commands {

    ProjectCommand::ProjectCommand(option_parser_pointer optionParser, event_handler_map eventHandlerMap) noexcept :
        m_optionParser{std::move(optionParser)},
        m_eventHandlerMap{std::move(eventHandlerMap)} {
        assert(static_cast<bool>(m_optionParser));
    }

    bool ProjectCommand::execute() noexcept {
        // For every option that is typed, we need to process the event through
        // the event handler map.
        auto options{m_optionParser->getOptions()};

        // If the help option is requested, we don't need to process all the other options.
        auto optionIt = std::find_if(std::begin(options), std::end(options), [](const option_parser::option_pointer optionPointer){
            return optionPointer->getLongName() == "help" && optionPointer->isSet();
        });

        if(optionIt != std::end(options)) {
            // The help option is typed, let's display it.
            printHelp(std::cout);
            gh_cmd::utility::ClearAllOptions(options);
            return true;
        }

        bool bIsSomeSet{};
        for(auto& option : options) {
            if(option->isSet() && m_eventHandlerMap.contains(option->getLongName())) {
                m_eventHandlerMap[option->getLongName()](option);
                bIsSomeSet = true;
            }
        }

        if(!bIsSomeSet) {
            // This means that no option is given, the user typed only the command
            // without option.
            // In this case we want to display the help page.
            // TODO: THIS IS HARD-CODED DEPENDENCY, REMOVE THIS.
            printHelp(std::cout);
        }

        gh_cmd::utility::ClearAllOptions(options);

        return true;
    }

    bool ProjectCommand::processInputOptions(const std::vector<string_type>& inputTokens) noexcept {
        m_optionParser->parse(inputTokens);
        return true;
    }

    void ProjectCommand::printHelp(help_ostream_type outputStream) const noexcept {
        outputStream.get() << "[NAME]" << std::endl;
        outputStream.get() << "\t" << strings::commands::PROJECT << " - manages the project of an automated greenhouse" << std::endl;
        outputStream.get() << "[SYNOPSIS]" << std::endl;
        outputStream.get() << "\t" << strings::commands::PROJECT << " [OPTION]..." << std::endl;
        outputStream.get() << "[DESCRIPTION]" << std::endl;
        outputStream.get() << "\t" << "Manages the project information and configuration of an automated greenhouse. It is" << std::endl;
        outputStream.get() << "\tcapable of handling the creation, loading and saving of the projects." << std::endl;
        outputStream.get() << "\tWARNING: Due to limitations of the underlying parsing engine, please DO NOT use ";
        outputStream.get() << "project titles with spaces." << std::endl;
        outputStream.get() << std::endl;

        m_optionParser->printHelp(outputStream);
    }

} // namespace rpi_gc::commands
