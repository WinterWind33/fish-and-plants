// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands/project-command.hpp>

namespace rpi_gc::commands {

    bool ProjectCommand::execute() noexcept {
        return false;
    }

    bool ProjectCommand::processInputOptions(const std::vector<string_type>& inputTokes) noexcept {
        return false;
    }

    void ProjectCommand::printHelp(help_ostream_type outputStream) const noexcept {

    }

} // namespace rpi_gc::commands
