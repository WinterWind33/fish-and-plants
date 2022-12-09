// Copyright (c) 2022 Andrea Ballestrazzi
#include "commands/abort-command.hpp"

namespace rpi_gc::commands {

    bool AbortCommand::execute() noexcept {
        return true;
    }

    void AbortCommand::printHelp(help_ostream_type outputStream) const noexcept {
    }

} // namespace rpi_gc::commands
