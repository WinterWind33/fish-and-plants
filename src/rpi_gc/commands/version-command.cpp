// Copyright (c) 2022 Andrea Ballestrazzi
#include <commands/version-command.hpp>
#include <rpi_gc-config-file.hpp>

namespace rpi_gc {

    VersionCommand::VersionCommand(ostream_ref outputStream) noexcept :
        m_outputStream{std::move(outputStream)} {}

    bool VersionCommand::processOptions(const options_vector& options, const non_options_vector& nonOptions, const unknown_options_vector& unknowns) noexcept {
        // This command shouldn't have any valid options.
        assert(options.empty());
        return true;
    }

    bool VersionCommand::execute() noexcept {
        constexpr std::string_view APPLICATION_NAME{"Greenhouse Controller"};

        m_outputStream.get() << APPLICATION_NAME << " - Version " <<
            rpi_gc_VERSION_MAJOR << "." << rpi_gc_VERSION_MINOR << "." << rpi_gc_VERSION_PATCH;
        m_outputStream.get() << std::endl;

        return true;
    }

} // namespace rpi_gc
