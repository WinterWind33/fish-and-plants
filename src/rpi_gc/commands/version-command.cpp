// Copyright (c) 2022 Andrea Ballestrazzi
#include <commands/version-command.hpp>
#include <rpi_gc-config-file.hpp>

namespace rpi_gc {

    VersionCommand::VersionCommand(ostream_ref outputStream) noexcept :
        m_outputStream{std::move(outputStream)},
        m_asOption{std::make_shared<gh_cmd::Switch<char_type>>('v', "version", "Displays the version of this application.")} {}

    bool VersionCommand::processOptions(const options_vector& options, const non_options_vector& nonOptions, const unknown_options_vector& unknowns) noexcept {
        // This command shouldn't have any valid options.
        assert(options.empty());
        return true;
    }

    bool VersionCommand::execute() noexcept {
        m_outputStream.get() << strings::application::NAME << " - Version " <<
            rpi_gc_VERSION_MAJOR << "." << rpi_gc_VERSION_MINOR << "." << rpi_gc_VERSION_PATCH;
        m_outputStream.get() << std::endl;

        return true;
    }

    void VersionCommand::printHelp(help_ostream_type& outputStream) const noexcept {
        outputStream << ">>> version" << std::endl;
        outputStream << "\tDescription: prints the name of the application and its version;";
    }

} // namespace rpi_gc
