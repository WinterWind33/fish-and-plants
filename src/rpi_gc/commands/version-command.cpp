// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands/version-command.hpp>
#include <version/version-numbers.hpp>

namespace rpi_gc {

VersionCommand::VersionCommand(ostream_ref outputStream) noexcept
    : m_outputStream{outputStream},
      m_asOption{std::make_shared<gh_cmd::Switch<char_type>>(
          'v', "version", "Displays the version of this application.")} {}

bool VersionCommand::execute() noexcept {
    m_outputStream.get() << strings::application::NAME << " - Version "
                         << version::getApplicationVersion() << std::endl;
    return true;
}

void VersionCommand::printHelp(help_ostream_type outputStream) const noexcept {
    outputStream.get() << "[NAME]" << std::endl;
    outputStream.get() << "\t" << strings::commands::VERSION
                       << " - Prints the name and the version of this application.";
}

} // namespace rpi_gc
