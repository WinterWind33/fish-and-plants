// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands/status-command.hpp>

namespace rpi_gc::commands {

    bool StatusCommand::execute() noexcept {
        return true;
    }

    bool StatusCommand::processInputOptions(const std::vector<string_type>& inputTockens) noexcept {
        return false;
    }

    void StatusCommand::printHelp(help_ostream_type outputStream) const noexcept {

    }


} // namespace rpi_gc::commands
