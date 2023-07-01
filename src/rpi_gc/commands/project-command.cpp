// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands/project-command.hpp>

// C++ STL
#include <cassert>

namespace rpi_gc::commands {

    ProjectCommand::ProjectCommand(option_parser_pointer optionParser) noexcept :
        m_optionParser{std::move(optionParser)} {
        assert(static_cast<bool>(m_optionParser));
    }

    bool ProjectCommand::execute() noexcept {
        return false;
    }

    bool ProjectCommand::processInputOptions(const std::vector<string_type>& inputTokens) noexcept {
        m_optionParser->parse(inputTokens);
        return true;
    }

    void ProjectCommand::printHelp(help_ostream_type outputStream) const noexcept {

    }

} // namespace rpi_gc::commands
