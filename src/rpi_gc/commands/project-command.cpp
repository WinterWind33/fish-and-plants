// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands/project-command.hpp>

// C++ STL
#include <cassert>

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

        for(auto& option : options) {
            if(option->isSet() && m_eventHandlerMap.contains(option->getLongName())) {
                m_eventHandlerMap[option->getLongName()](option);
            }

            option->clear();
        }

        return true;
    }

    bool ProjectCommand::processInputOptions(const std::vector<string_type>& inputTokens) noexcept {
        m_optionParser->parse(inputTokens);
        return true;
    }

    void ProjectCommand::printHelp(help_ostream_type outputStream) const noexcept {

    }

} // namespace rpi_gc::commands
